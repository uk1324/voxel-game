#include <Engine/Window.hpp>
#include <Log/Log.hpp>

Window::Window(int width, int height, const char* title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
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

void Window::swapBufers()
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
