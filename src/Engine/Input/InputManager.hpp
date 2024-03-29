#pragma once

#include <Engine/Window.hpp>
#include <Engine/Input/KeyCode.hpp>
#include <Engine/Input/MouseButton.hpp>
#include <Math/Vec2.hpp>

#include <memory>
#include <unordered_map>

class InputManager
{
public:
	InputManager(Window& window);
	~InputManager();

	void update();
	
	// Can only map a single action to a single key.
	void registerKeyboardButton(std::string name, KeyCode key);
	void registerMouseButton(std::string name, MouseButton button);

	// Also add functions for checking actual keyboard buttons. This is useful for debugging.
	bool isButtonDown(const std::string name) const;
	bool isButtonUp(const std::string name) const;
	bool isButtonHeld(const std::string name) const;

	const Vec2& mousePos() const;
	const Vec2& lastMousePos() const;
	float scrollOffset() const;

public:
	// Have to use this because you cannot convert a capturing lambda to a function pointer
	// and std::function does a virtual call
	static InputManager* self; // Maybe rename this to s_self so I don't have to write InputManager::self.
	static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

private:
	Window& m_window;

	std::unordered_map<KeyCode, std::string> m_keyToAction;

	std::unordered_map<MouseButton, std::string> m_mouseButtonToAction;

	// Would it be better to just store a struct with a bitset?
	std::unordered_map<std::string, bool> m_isButtonDown;
	std::unordered_map<std::string, bool> m_isButtonUp;
	std::unordered_map<std::string, bool> m_isButtonHeld;


	Vec2 m_mousePos;
	Vec2 m_lastMousePos;
	float m_scrollOffset;
};