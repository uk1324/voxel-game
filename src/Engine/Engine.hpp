#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Window.hpp>
#include <Engine/Time.hpp>

#include <string_view>
#include <unordered_map>
#include <memory>

class Engine
{
public:
	Engine(int updatesPerSecond, Window& window);

	void run(std::unique_ptr<Scene>&& startingScene);
	void update();
	void stop();

	void changeScene(std::unique_ptr<Scene> scene);

	Time& time();
	Window& window();

public:
	static Window init(int windowWidth, int windowHeight, std::string_view windowTitle);
	static void terminate();

private:
	static void initGlfw();
	static void initOpenGl();
	static void initImGui(Window& window);

	static void glfwErrorCallback(int errorCode, const char* errorMessage);
	static void APIENTRY openGlErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

public:
	static constexpr int OPENGL_VERSION_MAJOR = 4;
	static constexpr int OPENGL_VERSION_MINOR = 3;

private:
	static bool s_isInitialized;

private:
	Window& m_window;

	std::unique_ptr<Scene> m_currentScene;

	Time m_time;
	int m_updatesPerSecond;
};