#pragma once

#include <Game/Blocks/BlockSystem.hpp>
#include <Engine/Input/InputManager.hpp>
#include <Math/Quat.hpp>

#include <Context.hpp>
#include <Allocator.hpp>

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
public:
	struct PrintCommand
	{
		std::string text;
	};
	struct SetBlockCommand
	{
		Vec3I pos;
		int blockType;
	};
	struct SetPlayerPos
	{
		Vec3 pos;
	};
	struct GetPlayerPosCommand
	{
		Vec3& result;
	};
	struct GetPlayerDirCommand
	{
		Vec3& result;
	};
	struct RayCastCommand
	{
		Vec3 pos;
		Vec3 ray;
		std::optional<Vec3>& result;
	};
	using Command = std::variant<
		PrintCommand, 
		SetBlockCommand, 
		SetPlayerPos, 
		GetPlayerPosCommand, 
		GetPlayerDirCommand,
		RayCastCommand
	>;

public:
	Console();
	~Console();

	void update(Vec3& playerPos, const Quat& playerRot, BlockSystem& blocks, const InputManager& input);
	void draw();
private:
	void addLogItem(const char* format, ...);
	void addColoredLogItem(std::optional<ImVec4> color, const char* format, ...);
	void clearLog();
	void execute(std::string_view code);

	void textInputCallback(ImGuiInputTextCallbackData* data);
	static constexpr char* WHITESPACE = " \t\r\n";
	static std::string_view trim(std::string_view text);

	void runScriptingLanguage();

public:
	// Potentialy expensive copy.
	void addWriteCommand(Command command);
	void addReadCommand(Command command);

private:
	// Later add different kinds of entries for commands and output from the language and output from the repl
	// and change color based on that.
	struct LogItem
	{
		std::string text;
		std::optional<ImVec4> color;
	};
	std::vector<LogItem> logItems;

	std::vector<std::string> history;
	std::optional<size_t> historyIndex;
	std::string textBeforeLookingAtHistory;

public:
	std::vector<std::string> autocompleteWords;
private:

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
	// onUseHeld has to be destroted before the allocator.
	Voxl::Allocator allocator;
public:
	enum class Action
	{
		useHeld
	};
	std::optional<Voxl::LocalValue> onUseHeld;
	std::mutex actionsMutex;
	std::vector<Action> actions;
};