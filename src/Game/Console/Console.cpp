#include <Game/Console/Console.hpp>
#include <Utils/Assertions.hpp>

#include <imgui.h>
#include <algorithm>
#include <iostream>
#include <windows.h>

Console::Console()
    : inputTextBuffer("")
    , editorMode(false)
    , switchedInputs(false)
    , reclaimFocus(false)
    , autocompleteWords({ ".clear", ".edit", ".help", "for", "fn", "if", "ret", "class" })
{
    std::sort(autocompleteWords.begin(), autocompleteWords.end());

    addLogItem("Type \".help\" to display available commands.\n");
    commandQueue.push(PrintCommand{ "abc" });

    scriptingLanguageThread = std::thread(&Console::runScriptingLanguage, this);
}

Console::~Console()
{
    // I don't think there is any way to cooperatively exit the thread. There are no synchronization points that have to happen.
    // C++ std::thread doesn't provide any way to terminate a single thread. When the destructor is executed and the thread 
    // is not joined nor detached then std::terminate is called which shuts down the whole process.
    scriptingLanguageThread.detach(); // Call detatch so std::teminate is not called.
    TerminateThread(scriptingLanguageThread.native_handle(), EXIT_SUCCESS);
}

void Console::update()
{
    std::lock_guard lock(commandQueueMutex);
    while (commandQueue.empty() == false)
    {
        const auto command = std::move(commandQueue.front());
        commandQueue.pop();
        // Could unlock the mutex while processing to allow for more commands to be issued.
        std::visit([this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same<T, PrintCommand>())
            {
                addLogItem(arg.text.c_str());
            }
            else
            {
                ASSERT_NOT_REACHED();
            }
        }, command);
    }
    waitForReadCommandToFinish.notify_one();
}

void Console::draw()
{
    using namespace ImGui;
    SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    Begin("Console");

    Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = editorMode
        ? 100
        : GetStyle().ItemSpacing.y + GetFrameHeightWithSpacing();

    BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false);

    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    for (const auto& item : logItems)
    {
        //m_pixelTextUnformatted(item.c_str());
        TextWrapped(item.c_str());
        // Not sure if this should be here. Scripts can print empty strings.
        ASSERT(item.size() > 0);
        if (item.back() != '\n')
        {
            SameLine();
        }
        /*if (!Filter.PassFilter(item))
            continue;*/

        // Normally you would store more information in your item than just a string.
        // (e.g. make Items[] an array of structure, store color/type etc.)
        //ImVec4 color;
        //bool has_color = false;
        //if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
        //else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
        //if (has_color)
        //    PushStyleColor(ImGuiCol_Text, color);
        //TextUnformatted(item);
        //if (has_color)
        //    PopStyleColor();
    }

    static bool ScrollToBottom = true, AutoScroll = true;
    if (ScrollToBottom || (AutoScroll && GetScrollY() >= GetScrollMaxY()))
        SetScrollHereY(1.0f);
    ScrollToBottom = false;

    PopStyleVar();
    EndChild();
    Separator();

    // Calling SetKeyboardFocusHere(-1) after InputTextMultiline doesn't work, probably becuase it is made from multiple elements.
    // So the state needs to be transfered onto the next frame.

    // After submiting input the widget unfocues.
    if (reclaimFocus)
    {
        SetKeyboardFocusHere();
        reclaimFocus = false;
    }
    if (switchedInputs)
    {
        // Focus on the new input after switching inputs.
        SetKeyboardFocusHere();
        switchedInputs = false;
    }

    auto inputCallback = [](ImGuiInputTextCallbackData* data) -> int
    {
        reinterpret_cast<Console*>(data->UserData)->textInputCallback(data);
        return 0;
    };

    auto onInputSubmit = [this]
    {
        auto code = trim(std::string_view(inputTextBuffer));
        if (code.size() > 0)
        {
            auto codeString = std::string(code) + '\n';
            addLogItem("%s", codeString.c_str());
            execute(code);
            codeString.pop_back(); // Remove '\n'.
            history.push_back(std::move(codeString));
            historyIndex = std::nullopt;
        }
        strcpy(inputTextBuffer, "");
        reclaimFocus = true;
    };

    if (editorMode)
    {
        if(InputTextMultiline(
            "##",
            inputTextBuffer,
            IM_ARRAYSIZE(inputTextBuffer),
            ImVec2(0, 0),
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion,
            inputCallback,
            reinterpret_cast<void*>(this)))
        {
            onInputSubmit();
        }
    }
    else
    {
        if (InputText(
            "##replInput",
            inputTextBuffer,
            IM_ARRAYSIZE(inputTextBuffer),
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory,
            inputCallback,
            reinterpret_cast<void*>(this)))
        {
            onInputSubmit();
        }
    }

    // TODO: Imgui doesn't check if keys like U are pressed so it has to be handled by the engine input system.
    /*if (GetIO().KeyCtrl && IsKeyDown(ImGuiKey_V)
    {
        strcpy(inputBuffer, "");
    }*/
    // Don't know what this does. It doesn't focus on the last elmenet by default.
    //SetItemDefaultFocus();

    End();
}

void Console::addLogItem(const char* format, ...)
{
    char text[1024];

    va_list args;
    va_start(args, format);
    const auto returnValue = vsnprintf(text, IM_ARRAYSIZE(text), format, args);
    va_end(args);
    ASSERT(returnValue >= 0);
    if (returnValue > (IM_ARRAYSIZE(text) - 1 /* null byte */))
    {
        strcpy(text + IM_ARRAYSIZE(text) - 1 - 3, "...");
    }

    logItems.push_back(std::move(text));
}

void Console::clearLog()
{
    logItems.clear();
}

void Console::execute(std::string_view code)
{
    ASSERT(code.size() > 0);
    if (code[0] == '.')
    {
        const auto command = code.substr(1);
        if (command == "help")
        {
            #define JUSTIFY "-10"
            addLogItem(
                "%" JUSTIFY "s clear the log\n"
                "%" JUSTIFY "s switch to and from eidtor mode\n",
            // For this to look good justify must be bigger than the command lengths.
                ".clear",
                ".editor");
            #undef LEFT
        }
        else if (command == "clear")
        {
            clearLog();
        }
        else if (command == "editor")
        {
            switchedInputs = true;
            editorMode = !editorMode;
        }
        else
        {
            addLogItem("unknown command\n");
        }
        return;
    }

    std::lock_guard lock(codeQueueMutex);
    codeQueue.push(std::string(code));
    waitForCodeOnQueue.notify_one();
}

void Console::textInputCallback(ImGuiInputTextCallbackData* data)
{
    switch (data->EventFlag)
    {
    case ImGuiInputTextFlags_CallbackCompletion:
    {
        const std::string_view buffer(data->Buf, data->CursorPos);
        const auto wordStartOrNpos = buffer.find_last_of(WHITESPACE);
        const auto wordStart = (wordStartOrNpos == std::string_view::npos) ? 0 : (wordStartOrNpos + 1);
        const auto word = buffer.substr(wordStart);

        std::vector<const std::string*> candidates; // Could allocate inside class.
        for (const auto& candidate : autocompleteWords)
        {
            // Could use std::mismatch
            if ((candidate.find(word) == 0) && (candidate.length() != word.length()))
            {
                candidates.push_back(&candidate);
            }
        }

        if (candidates.size() == 1)
        {
            const auto& candidate = *candidates[0];
            const auto prefixLength = candidate.length() - (candidate.length() - word.length());
            data->InsertChars(data->CursorPos, candidate.data() + prefixLength, candidate.data() + candidate.length());
        }
        else if (candidates.size() > 1)
        {
            std::string text;
            for (const auto candidate : candidates)
            {
                text += *candidate;
                text += ' ';
            }
            text += '\n';
            addLogItem("%s", text.c_str());
        }
        break;
    }

    case ImGuiInputTextFlags_CallbackHistory:
    {
        if (history.size() == 0)
            break;

        const auto previousHistoryPos = historyIndex;
        if (data->EventKey == ImGuiKey_UpArrow)
        {
            if (historyIndex.has_value() == false)
                historyIndex = history.size() - 1;
            else if ((*historyIndex) > 0)
                (*historyIndex)--;
        }
        else if (data->EventKey == ImGuiKey_DownArrow)
        {
            if (historyIndex.has_value())
            {
                if (*historyIndex + 1 >= history.size())
                {
                    historyIndex = std::nullopt;
                }
                else
                {
                    (*historyIndex)++;
                }
            }
        }

        if (previousHistoryPos != historyIndex)
        {
            if (previousHistoryPos.has_value() == false)
                textBeforeLookingAtHistory = std::string(data->Buf, data->BufTextLen);

            // TODO how to handle newlines in textBeforeLookingAtHistory.
            // Should they be revmoed when in replMode?
            const auto& text = historyIndex.has_value() ? history[*historyIndex] : textBeforeLookingAtHistory;
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, text.c_str());
        }
        break;
    }

    }
}

std::string_view Console::trim(std::string_view text)
{
    const auto start = text.find_first_not_of(WHITESPACE);
    const auto end = text.find_last_not_of(WHITESPACE);
    return text.substr(start, end - start + 1);
}

// Cannot allocate any resources on this thread because they won't get properly cleaned up when it is interrupted and terminated.
// What happens to thread_local variables?
void Console::runScriptingLanguage()
{
    for (;;)
    {
        std::unique_lock lock(codeQueueMutex);
        waitForCodeOnQueue.wait(lock, [this] { return codeQueue.empty() == false; });
        const auto code = std::move(codeQueue.front());
        codeQueue.pop();
        lock.unlock();
           
        if (trim(std::string_view(code)) == "print")
        {
            commandQueue.push(PrintCommand{ "cool\n" });
        }
    }
}

void Console::addWriteCommand(Command command)
{
    std::lock_guard lock(commandQueueMutex);
    commandQueue.push(std::move(command));
}

void Console::addReadCommand(Command command)
{
    std::unique_lock lock(commandQueueMutex);
    commandQueue.push(std::move(command));
    // Could check if size < sizeBefore though that would have the same effect as empty() in this case.
    waitForReadCommandToFinish.wait(lock, [this] { return commandQueue.empty(); }); 
}
