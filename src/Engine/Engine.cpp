#include <Engine/Engine.hpp>
#include <Engine/Init.hpp>
#include <Engine/Window.hpp>
#include <Utils/Assertions.hpp>

Engine::Engine(int windowWidth, int windowHeight, std::string_view windowTitle)
	: m_currentScene(nullptr)
{
	initGlfw();
	// Have to create a window before initializing OpenGL.

	m_window = Window(windowWidth, windowHeight, windowTitle);

	initOpenGl();
}

Engine::~Engine()
{
	glfwTerminate();
}

void Engine::run(std::string_view startingScene)
{
	changeScene(startingScene);
	m_isRunning = true;

	while (m_isRunning)
	{
		m_currentScene->update();


		
		m_window.update();
		glfwPollEvents();
	}
}

void Engine::stop()
{
	m_isRunning = false;
}

void Engine::addScene(std::string_view name, std::unique_ptr<Scene>&& scene)
{
	m_scenes[name] = std::move(scene);
}

void Engine::changeScene(std::string_view name)
{
	ASSERT(m_scenes.count(name) != 0);

	m_currentScene = m_scenes[name].get();
}
