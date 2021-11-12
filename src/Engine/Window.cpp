#include <Engine/Window.hpp>
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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



void Window::update()
{
	glfwSwapBuffers(m_handle);
}

void Window::close()
{
	glfwSetWindowShouldClose(m_handle, true);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_handle);
}

GLFWwindow* Window::handle()
{
	return m_handle;
}
