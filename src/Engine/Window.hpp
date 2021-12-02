#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string_view>

class Window
{
public:
	// The default and move constructor and move assignment operator only exists because you have to
	// call GLFWinit before constructing a window so it can't be created in the initializer list.
	Window();
	Window(Window&& window) noexcept;
	Window(int width, int height, std::string_view title);
	~Window();
	Window(const Window&) = delete;
	Window& operator= (const Window&) = delete;
	Window& operator= (Window&& window) noexcept;

	void getWindowSize(int& width, int& height);

	void update();
	void close();

	bool shouldClose();

	GLFWwindow* handle();

private:
	GLFWwindow* m_handle;
};