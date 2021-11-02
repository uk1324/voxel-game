#include <Engine/Input/InputManager.hpp>

#include <functional>

InputManager* InputManager::self = nullptr;

InputManager::InputManager(Window& window)
	: m_window(window)
{
	InputManager::self = this;
	glfwSetKeyCallback(window.handle(), InputManager::keyboardCallback);
	glfwSetCursorPosCallback(window.handle(), InputManager::mouseMoveCallback);
	glfwSetMouseButtonCallback(window.handle(), InputManager::mouseButtonCallback);
}

InputManager::~InputManager()
{
	glfwSetKeyCallback(m_window.handle(), nullptr);
	glfwSetCursorPosCallback(m_window.handle(), nullptr);
	glfwSetMouseButtonCallback(m_window.handle(), nullptr);
}

void InputManager::update()
{
	glfwPollEvents();
	updateKeyboard();
	updateMouseButtons();
}

void InputManager::registerKeyboardAction(KeyCode key, std::unique_ptr<ButtonAction>&& action)
{
	m_keyboardActions[key] = std::move(action);
	m_isKeyHeld[key] = false;
}

void InputManager::registerMouseMoveAction(std::unique_ptr<MouseMoveAction>&& action)
{
	m_mouseMoveActions.push_back(std::move(action));
}

void InputManager::registerMouseButtonAction(MouseButton button, std::unique_ptr<ButtonAction>&& action)
{
	m_mouseButtonActions[button] = std::move(action);
	m_isMouseButtonHeld[button] = false;
}

void InputManager::updateKeyboard()
{
	for (auto& [key, isHeld] : m_isKeyHeld)
	{
		if (isHeld)
			m_keyboardActions[key]->buttonHeld();
	}
}

void InputManager::updateMouseButtons()
{
	for (auto& [button, isHeld] : m_isMouseButtonHeld)
	{
		if (isHeld)
			m_mouseButtonActions[button]->buttonHeld();
	}
}

void InputManager::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto value = InputManager::self->m_keyboardActions.find(static_cast<KeyCode>(key));
	if (value != InputManager::self->m_keyboardActions.end())
	{
		if (action == GLFW_PRESS)
		{
			value->second->buttonDown();
			InputManager::self->m_isKeyHeld[static_cast<KeyCode>(key)] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			value->second->buttonUp();
			InputManager::self->m_isKeyHeld[static_cast<KeyCode>(key)] = false;
		}
	}
}

void InputManager::mouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY)
{
	for (const auto& action : InputManager::self->m_mouseMoveActions)
	{
		action->execute(mouseX, mouseY);
	}
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto value = InputManager::self->m_mouseButtonActions.find(static_cast<MouseButton>(button));
	if (value != InputManager::self->m_mouseButtonActions.end())
	{
		if (action == GLFW_PRESS)
		{
			value->second->buttonDown();
			InputManager::self->m_isMouseButtonHeld[static_cast<MouseButton>(button)] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			value->second->buttonUp();
			InputManager::self->m_isMouseButtonHeld[static_cast<MouseButton>(button)] = false;
		}
	}
}
