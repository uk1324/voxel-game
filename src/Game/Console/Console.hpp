#pragma once

#include <string>
#include <vector>
#include <optional>
#include <queue>
#include <thread>
#include <mutex>
#include <imgui.h>
#include <variant>

class Console
{
private:
	struct PrintCommand
	{
		std::string text;
	};
	/*struct ReadCommand
	{
		result* result;
	};*/
	using Command = std::variant<PrintCommand>;

public:
	Console();
	~Console();

	void update();
	void draw();
private:
	void addLogItem(const char* format, ...);
	void clearLog();
	void execute(std::string_view code);

	void textInputCallback(ImGuiInputTextCallbackData* data);
	static constexpr char* WHITESPACE = " \t\r\n";
	static std::string_view trim(std::string_view text);

	void runScriptingLanguage();
	// Potentialy expensive copy.
	void addWriteCommand(Command command);
	void addReadCommand(Command command);

private:
	// Later add different kinds of entries for commands and output from the language and output from the repl
	// and change color based on that.
	std::vector<std::string> logItems;

	std::vector<std::string> history;
	std::optional<size_t> historyIndex;
	std::string textBeforeLookingAtHistory;

	std::vector<std::string> autocompleteWords;

	char inputTextBuffer[1024];
	bool editorMode; // Maybe use enum?
	bool switchedInputs;
	bool reclaimFocus;


	std::mutex codeQueueMutex;
	std::queue<std::string> codeQueue;
	std::condition_variable waitForCodeOnQueue;

	std::mutex commandQueueMutex;
	std::queue<Command> commandQueue;
	std::condition_variable waitForReadCommandToFinish;

	std::thread scriptingLanguageThread;
};