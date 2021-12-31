#include <Engine/Input/InputManager.hpp>
#include <Utils/Assertions.hpp>

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
	for (auto& [button, isDown] : m_isButtonDown)
		isDown = false;

	for (auto& [button, isUp] : m_isButtonUp)
		isUp = false;

	m_lastMousePos = m_mousePos;

	glfwPollEvents();
}

void InputManager::registerKeyboardButton(std::string name, KeyCode key)
{
	m_isButtonDown[name] = false;
	m_isButtonUp[name] = false;
	m_isButtonHeld[name] = false;
	m_keyToAction[key] = std::move(name);
}

void InputManager::registerMouseButton(std::string name, MouseButton button)
{
	m_isButtonDown[name] = false;
	m_isButtonUp[name] = false;
	m_isButtonHeld[name] = false;
	m_mouseButtonToAction[button] = std::move(name);
}

bool InputManager::isButtonDown(const std::string name) const
{
	return m_isButtonDown.at(name);
}

bool InputManager::isButtonUp(const std::string name) const
{
	return m_isButtonUp.at(name);
}

bool InputManager::isButtonHeld(const std::string name) const
{
	return m_isButtonHeld.at(name);
}

const Vec2& InputManager::mousePos() const
{
	return m_mousePos;
}

const Vec2& InputManager::lastMousePos() const
{
	return m_lastMousePos;
}

void InputManager::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto button = InputManager::self->m_keyToAction.find(static_cast<KeyCode>(key));
	if (button != InputManager::self->m_keyToAction.end())
	{
		if (action == GLFW_PRESS)
		{
			InputManager::self->m_isButtonDown[button->second] = true;
			InputManager::self->m_isButtonHeld[button->second] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			InputManager::self->m_isButtonUp[button->second] = true;
			InputManager::self->m_isButtonHeld[button->second] = false;
		}
	}
}

void InputManager::mouseMoveCallback(GLFWwindow* window, double mouseX, double mouseY)
{
	InputManager::self->m_mousePos = Vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto value = InputManager::self->m_mouseButtonToAction.find(static_cast<MouseButton>(button));
	if (value != InputManager::self->m_mouseButtonToAction.end())
	{
		if (action == GLFW_PRESS)
		{
			InputManager::self->m_isButtonDown[value->second] = true;
			InputManager::self->m_isButtonHeld[value->second] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			InputManager::self->m_isButtonUp[value->second] = true;
			InputManager::self->m_isButtonHeld[value->second] = false;
		}
	}
}
