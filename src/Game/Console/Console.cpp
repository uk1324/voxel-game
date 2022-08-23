#include <Game/Console/Console.hpp>
#include <Engine/Input/InputManager.hpp>
#include <Utils/Assertions.hpp>
#include <Engine/Time.hpp>

#include <Vm/Vm.hpp>
#include <TerminalErrorReporter.hpp>

#include <imgui.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <windows.h>

using namespace Voxl;
using namespace std::chrono_literals;

namespace
{

class ConsoleErrorReporter final : public ErrorReporter
{
public:
    ConsoleErrorReporter(Console& console, const SourceInfo& sourceInfo, size_t tabWidth)
        : console(console)
        , terminalErrorReporter(out, sourceInfo, tabWidth)
    {}

    void printError()
    {
        console.addWriteCommand(Console::PrintCommand{ out.str() });
        out.str(std::string());
    }

    void onScannerError(const SourceLocation& location, std::string_view message) override
    {
        terminalErrorReporter.onScannerError(location, message);
        printError();
    }

    void onParserError(const Token& token, std::string_view message) override
    {
        terminalErrorReporter.onParserError(token, message);
        printError();
    }

    void onCompilerError(const SourceLocation& location, std::string_view message) override
    {
        terminalErrorReporter.onCompilerError(location, message);
        printError();
    }

    void onVmError(const Vm& vm, std::string_view message) override
    {
        terminalErrorReporter.onVmError(vm, message);
        printError();
    }

    void onUncaughtException(const Vm& vm, std::optional<std::string_view> exceptionTypeName, std::optional<std::string_view> message) override
    {
        terminalErrorReporter.onUncaughtException(vm, exceptionTypeName, message);
        printError();
    }

    Console& console;
    std::stringstream out;
    TerminalErrorReporter terminalErrorReporter;
};

}

// It would be cool if you could select an object in game and the access the selected object in the language.

static Vec3 readVec3(LocalValue& vec)
{
    return Vec3(vec.get("x").asNumber(), vec.get("y").asNumber(), vec.get("z").asNumber());
}

static constexpr int putArgCount = 1;
static LocalValue put(Context& c)
{
    auto& console = *reinterpret_cast<Console*>(c.data);
    auto arg = c.args(0);
    auto value = arg.value;
    std::stringstream out;
    if (value.isObj() && value.asObj()->isInstance())
    {
        auto strinfigy = arg.at("$str");
        if (strinfigy.has_value())
        {
            out << (*strinfigy)().asString().chars();
        }
        else
        {
            out << value;
        }
    }
    else
    {
        out << value;
    }
    out << '\n';
    console.addWriteCommand(Console::PrintCommand{ out.str() });
    return LocalValue::null(c);
}

static constexpr int setBlockArgCount = 2;
static LocalValue set_block(Context& c)
{
    auto& console = *reinterpret_cast<Console*>(c.data);
    auto posArg = c.args(0);
    const auto pos = Vec3I(readVec3(posArg).applied(floor));
    const auto blockType = static_cast<int>(c.args(1).asNumber());
    console.addWriteCommand(Console::SetBlockCommand{ pos, blockType });
    return LocalValue::null(c);
}

static constexpr int setPlayerPos = 1;
static LocalValue set_player_pos(Context& c)
{
    auto& console = *reinterpret_cast<Console*>(c.data);
    auto pos = readVec3(c.args(0));
    console.addWriteCommand(Console::SetPlayerPos{ pos });
    return LocalValue::null(c);
}

static constexpr int playerPosArgCount = 0;
static LocalValue player_pos(Context& c)
{
    auto& console = *reinterpret_cast<Console*>(c.data);
    Vec3 playerPos;
    console.addReadCommand(Console::GetPlayerPosCommand{ playerPos });
    return c.get("Vec3")(LocalValue::floatNum(playerPos.x, c), LocalValue::floatNum(playerPos.y, c), LocalValue::floatNum(playerPos.z, c));
}

static constexpr int playerDirArgCount = 0;
static LocalValue player_dir(Context& c)
{
    auto& console = *reinterpret_cast<Console*>(c.data);
    Vec3 playerDir;
    console.addReadCommand(Console::GetPlayerDirCommand{ playerDir });
    return c.get("Vec3")(LocalValue::floatNum(playerDir.x, c), LocalValue::floatNum(playerDir.y, c), LocalValue::floatNum(playerDir.z, c));
}

static constexpr int raycastArgCount = 2;
static LocalValue raycast(Context& c)
{
    auto& console = *reinterpret_cast<Console*>(c.data);
    std::optional<Vec3> hit;
    auto pos = readVec3(c.args(0));
    auto dir = readVec3(c.args(1));
    console.addReadCommand(Console::RayCastCommand{ pos, dir, hit});
    if (hit.has_value())
    {
        return c.get("Vec3")(LocalValue::floatNum(hit->x, c), LocalValue::floatNum(hit->y, c), LocalValue::floatNum(hit->z, c));
    }
    return LocalValue::null(c);
}

static constexpr int setOnUseHeldArgCount = 1;
static LocalValue set_on_use_held(Context& c)
{
    auto& console = *reinterpret_cast<Console*>(c.data);
    auto function = c.args(0);
    if (function.value.isObj() && function.value.asObj()->isFunction())
    {
        console.onUseHeld = function;
    }
    else
    {
        console.onUseHeld = std::nullopt;
    }
    return LocalValue::null(c);
}

static LocalValue gameModuleMain(Context& c)
{
    auto& console = *reinterpret_cast<Console*>(c.data);
    c.useAllFromModule("./src/Game/Console/utils.voxl");

    auto addFn = [c, &console](std::string_view name, NativeFunction function, int argCount) mutable 
    {
        c.createFunction(name, function, argCount, c.data);
        // TODO: Synchronize.
        console.autocompleteWords.push_back(std::string(name));
    };

    addFn("put", put, putArgCount);
    addFn("set_block", set_block, setBlockArgCount);
    addFn("set_player_pos", set_player_pos, setPlayerPos);
    addFn("player_pos", player_pos, playerPosArgCount);
    addFn("player_dir", player_dir, playerDirArgCount);
    addFn("set_on_use_held", set_on_use_held, setOnUseHeldArgCount);
    addFn("raycast", raycast, raycastArgCount);
    return LocalValue::null(c);
}

Console::Console()
    : inputTextBuffer("")
    , editorMode(false)
    , switchedInputs(false)
    , reclaimFocus(false)
    , autocompleteWords({ ".clear", ".edit", ".help", "for", "fn", "if", "ret", "class" })
{
    std::sort(autocompleteWords.begin(), autocompleteWords.end());

    addLogItem("Type \".help\" to display available commands.\n");
    codeQueue.push("use \"game\" -> *;");

    scriptingLanguageThread = std::thread(&Console::runScriptingLanguage, this);
}

Console::~Console()
{
    memset(&onUseHeld, 0, sizeof(onUseHeld));
    //onUseHeld = std::nullopt;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(500ms);
    std::exit(0);
    // terminate becuase for some reason the language thread isn't terminated.
    // I don't think there is any way to cooperatively exit the thread. There are no synchronization points that have to happen.
    // C++ std::thread doesn't provide any way to terminate a single thread. When the destructor is executed and the thread 
    // is not joined nor detached then std::terminate is called which shuts down the whole process.
    scriptingLanguageThread.detach(); // Call detatch so std::teminate is not called.
    TerminateThread(scriptingLanguageThread.native_handle(), EXIT_SUCCESS);
}

void Console::update(Vec3& playerPos, const Quat& playerRot, BlockSystem& blocks, const InputManager& input)
{
    {
        std::lock_guard lock(actionsMutex);
        if (Time::currentTick() % 4 == 0)
        {
            if (input.isButtonHeld("use"))
            {
                if (std::find(actions.begin(), actions.end(), Action::useHeld) == actions.end())
                    actions.push_back(Action::useHeld);
            }
        }
    }

    {
        std::lock_guard lock(commandQueueMutex);
        const auto start = std::chrono::high_resolution_clock::now();
        size_t count = 0;
        while (commandQueue.empty() == false)
        {
            const auto command = std::move(commandQueue.front());
            commandQueue.pop();
            // Could unlock the mutex while processing to allow for more commands to be issued.
            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same<T, PrintCommand>())
                {
                    std::string_view text = arg.text;
                    size_t offset = 0;
                    std::optional<ImVec4> color;
                    for (;;)
                    {
                        static constexpr std::string_view escapeSequence = "\033[";
                        const auto escapeSequenceOffset = text.find_first_of(escapeSequence, offset);
                        if (escapeSequenceOffset == std::string_view::npos)
                        {
                            addLogItem("%s", arg.text.c_str() + offset);
                            break;
                        }

                        std::string_view afterEscapeSequence = text.substr(escapeSequenceOffset + escapeSequence.size());
                        std::optional<ImVec4> newColor;
                        size_t oldOffset;
                        // +1 because there is a 'm' at the end.
                        if ((afterEscapeSequence.size() >= 1) && (afterEscapeSequence[0] == '0'))
                        {
                            newColor = std::nullopt;
                            oldOffset = offset;
                            offset = escapeSequenceOffset + escapeSequence.size() + 1 + 1;
                        }
                        else if ((afterEscapeSequence.size() >= 2) && (afterEscapeSequence[0] == '3'))
                        {
                            switch (afterEscapeSequence[1])
                            {
                            case '0': newColor = ImVec4(0, 0, 0, 1); break; // Black
                            case '1': newColor = ImVec4(1, 0, 0, 1); break; // Red
                            case '2': newColor = ImVec4(0, 1, 0, 1); break; // Green
                            case '3': newColor = ImVec4(1, 1, 0, 1); break; // Yellow
                            case '4': newColor = ImVec4(0, 0, 1, 1); break; // Blue
                            case '5': newColor = ImVec4(1, 0, 1, 1); break; // Magenta
                            case '6': newColor = ImVec4(0, 1, 1, 1); break; // Cyan
                            case '7': newColor = ImVec4(1, 1, 1, 1); break; // White
                            default:
                                // TODO: Do something on default.
                                break;
                            }
                            oldOffset = offset;
                            offset = escapeSequenceOffset + escapeSequence.size() + 2 + 1;
                        }
                        else
                        {
                            oldOffset = offset;
                            offset = escapeSequenceOffset;
                        }
                        const auto textPart = std::string_view(text.data() + oldOffset, escapeSequenceOffset - oldOffset);
                        addColoredLogItem(color, "%.*s", textPart.size(), textPart.data());
                        color = newColor;
                    }
                }
                else if constexpr (std::is_same<T, SetBlockCommand>())
                {
                    if ((arg.blockType > 0) && (arg.blockType < static_cast<int>(BlockType::Count)))
                    {
                        blocks.trySet(arg.pos, static_cast<BlockType>(arg.blockType));
                    }
                }
                else if constexpr (std::is_same<T, SetPlayerPos>())
                {
                    playerPos = arg.pos;
                }
                else if constexpr (std::is_same<T, GetPlayerPosCommand>())
                {
                    arg.result = playerPos;
                }
                else if constexpr (std::is_same<T, GetPlayerDirCommand>())
                {
                    arg.result = playerRot * Vec3::forward;
                }
                else if constexpr (std::is_same<T, RayCastCommand>())
                {
                    const auto hit = blocks.raycast(arg.pos, arg.pos + arg.ray);
                    if (hit.has_value())
                    {
                        arg.result = Vec3(hit->blockPos);
                    }
                    else
                    {
                        arg.result = std::nullopt;
                    }
                }
                else
                {
                    ASSERT_NOT_REACHED();
                }
                }, command);

            count++;
            if (count > 1000)
                break;
            /*const auto now = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() << '\n';
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > 2)
            {
                break;
            }*/
        }
        waitForReadCommandToFinish.notify_one();
    }
}

void Console::draw()
{
    using namespace ImGui;
    SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    Begin("Console");

    Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    ImGui::SetWindowFontScale(1.5);
    const float footer_height_to_reserve = editorMode
        ? 100
        : GetStyle().ItemSpacing.y + GetFrameHeightWithSpacing();

    BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false);

    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    for (const auto& [text, color] : logItems)
    {
        if (color.has_value())
            PushStyleColor(ImGuiCol_Text, *color);

        TextWrapped(text.c_str());
        if (text.size() == 0 || text.back() != '\n')
            SameLine();

        if (color.has_value())
            PopStyleColor();
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

    logItems.push_back({ std::move(text), std::nullopt });
}

void Console::addColoredLogItem(std::optional<ImVec4> color, const char* format, ...)
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

    logItems.push_back({ std::move(text), color });
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
        static constexpr auto NON_WORD_CHARACTER = " \t\r\n(){},";
        const auto wordStartOrNpos = buffer.find_last_of(NON_WORD_CHARACTER);
        const auto wordStart = (wordStartOrNpos == std::string_view::npos) ? 0 : (wordStartOrNpos + 1);
        const auto word = buffer.substr(wordStart);

        if (word.length() == 0)
        {
            data->InsertChars(data->CursorPos, "\t");
            break;
        }

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
            std::string_view commonPrefix = *candidates[0];
            for (size_t i = 1; i < candidates.size(); i++)
            {
                auto candidate = std::string_view(*candidates[i]);
                if (candidate.size() < commonPrefix.size())
                {
                    commonPrefix = commonPrefix.substr(0, candidate.size());
                }
                for (size_t j = 0; j < commonPrefix.size(); j++)
                {
                    if (commonPrefix[j] != candidate[j])
                    {
                        commonPrefix = commonPrefix.substr(0, j);
                        break;
                    }
                }
                // TODO: Comparing to word.size() would work better.
                if (commonPrefix.size() == 0)
                {
                    break;
                }
            }

            const auto wordPostfixSize = commonPrefix.size() - word.size();
            const auto postfixStart = commonPrefix.data() + word.size();
            data->InsertChars(data->CursorPos, postfixStart, postfixStart + wordPostfixSize);
            
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
    SourceInfo sourceInfo;
    sourceInfo.displayedFilename = "<repl>";
    sourceInfo.workingDirectory = std::filesystem::current_path();

    ConsoleErrorReporter errorReporter(*this, sourceInfo, 4);
    Parser parser(false);
    Scanner scanner;
    Compiler compiler(allocator);
    auto vm = std::make_unique<Vm>(allocator);
    vm->createModule("game", gameModuleMain, this);
    std::optional<ObjModule*> module;

    for (;;)
    {
        {
            std::unique_lock lock(actionsMutex);
            while (actions.empty() == false)
            {
                const auto action = actions.back();
                if (action == Action::useHeld && onUseHeld.has_value())
                {
                    lock.unlock();
                    vm->execute(onUseHeld->value.asObj()->asFunction(), *module, scanner, parser, compiler, sourceInfo, errorReporter);
                    lock.lock();
                }
                actions.pop_back();
            }
        }

        {
            std::unique_lock lock(codeQueueMutex);
            if (codeQueue.empty())
            {
                continue;
            }
            const auto code = std::move(codeQueue.front());
            codeQueue.pop();
            lock.unlock();

            sourceInfo.source = code;
            sourceInfo.lineStartOffsets.clear();

            const auto scannerResult = scanner.parse(sourceInfo, errorReporter);
            if (scannerResult.hadError)
                continue;

            const auto parserResult = parser.parse(scannerResult.tokens, sourceInfo, errorReporter);
            if (parserResult.hadError)
                continue;

            const auto compilerResult = compiler.compile(parserResult.ast, sourceInfo, errorReporter, module);
            module = compilerResult.module;

            if (compilerResult.hadError)
                continue;

            const auto vmResult = vm->execute(compilerResult.program, compilerResult.module, scanner, parser, compiler, sourceInfo, errorReporter);
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
