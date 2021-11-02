#pragma once

#include <Engine/Window.hpp>
#include <Engine/Input/ButtonAction.hpp>
#include <Engine/Input/MouseMoveAction.hpp>
#include <Engine/Input/KeyCode.hpp>
#include <Engine/Input/MouseButton.hpp>

#include <memory>
#include <unordered_map>

class InputManager
{
public:
	InputManager(Window& window);
	~InputManager();

	void update();

	void registerKeyboardAction(KeyCode key, std::unique_ptr<ButtonAction>&& action);
	void registerMouseMoveAction(std::unique_ptr<MouseMoveAction>&& action);
	void registerMouseButtonAction(MouseButton button, std::unique_ptr<ButtonAction>&& action);

private:
	void updateKeyboard();
	void updateMouseButtons();

private:
	// Have to use this because you cannot convert a capturing lambda to a function pointer
	// and std::function does a virtual call
	static InputManager* self;
	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:
	Window& m_window;

	std::unordered_map<KeyCode, std::unique_ptr<ButtonAction>> m_keyboardActions;
	std::unordered_map<KeyCode, bool> m_isKeyHeld;

	std::unordered_map<MouseButton, std::unique_ptr<ButtonAction>> m_mouseButtonActions;
	std::unordered_map<MouseButton, bool> m_isMouseButtonHeld;

	std::vector<std::unique_ptr<MouseMoveAction>> m_mouseMoveActions;
};