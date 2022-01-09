#pragma once

#include <Math/Vec2.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string_view>

class Window
{
public:
	Window();
	Window(Window&& window) noexcept;
	Window(int width, int height, std::string_view title);
	~Window();
	Window(const Window&) = delete;
	Window& operator= (const Window&) = delete;
	Window& operator= (Window&& window) noexcept;

	Vec2I getWindowSize();

	void update();
	void close();

	void setTitle(const char* title);
	void setPos(Vec2I pos);
	void hideCursor();
	void showCursor();

	bool shouldClose();

	GLFWwindow* handle();

private:
	GLFWwindow* m_handle;
};