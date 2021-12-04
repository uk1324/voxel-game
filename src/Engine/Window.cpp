#include <Engine/Window.hpp>
#include <Engine/Engine.hpp>
#include <Log/Log.hpp>

Window::Window()
	: m_handle(nullptr)
{}

Window::Window(Window&& window) noexcept
	: m_handle(window.m_handle)
{
	window.m_handle = nullptr;
}

Window::Window(int width, int height, std::string_view title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Engine::OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Engine::OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_handle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
	if (m_handle == nullptr)
	{
		LOG_FATAL("failed to create window");
	}
	glfwMakeContextCurrent(m_handle);
}

Window::~Window()
{
	glfwDestroyWindow(m_handle);
}

Window& Window::operator=(Window&& window) noexcept
{
	m_handle = window.m_handle;
	window.m_handle = nullptr;
	return *this;
}

Vec2I Window::getWindowSize()
{
	Vec2I size;
	glfwGetFramebufferSize(m_handle, &size.x, &size.y);
	return size;
}

void Window::update()
{
	glfwSwapBuffers(m_handle);
}

void Window::close()
{
	glfwSetWindowShouldClose(m_handle, true);
}

void Window::hideCursor()
{
	glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::showCursor()
{
	glfwSetInputMode(m_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_handle);
}

GLFWwindow* Window::handle()
{
	return m_handle;
}
