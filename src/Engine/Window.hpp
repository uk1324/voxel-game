#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();
	Window(const Window&) = delete;

	void swapBufers();
	void close();

	bool shouldClose();

	GLFWwindow* handle();

private:
	GLFWwindow* m_handle;
};