	#include <Engine/Engine.hpp>
#include <Engine/Init.hpp>
#include <Engine/Window.hpp>
#include <Utils/Assertions.hpp>

Engine::Engine(int updatesPerSecond, int windowWidth, int windowHeight, std::string_view windowTitle)
	: m_currentScene(nullptr)
	, m_updatesPerSecond(updatesPerSecond)
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

#include <iostream>
#include <chrono>
static auto start_time = std::chrono::high_resolution_clock::now();

static double getTime()
{
	auto current_time = std::chrono::high_resolution_clock::now();

	return std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - start_time).count() / 1000000000.0;
}

void Engine::run(std::string_view startingScene)
{
	changeScene(startingScene);

	constexpr double FRAME_TIME_CAP = 2.0;

	double lastFrameStartTime = glfwGetTime();
	double frameStartTime;
	double frameTime;

	double accumulatedTime = 0.0;
	const double updateTime = 1.0 / m_updatesPerSecond;

	while (m_window.shouldClose() == false)
	{
		frameStartTime = glfwGetTime();
		frameTime = frameStartTime - lastFrameStartTime;
		lastFrameStartTime = frameStartTime;

		if (frameTime > FRAME_TIME_CAP)
			frameTime = FRAME_TIME_CAP;

		accumulatedTime += frameTime;

		while (accumulatedTime >= updateTime)
		{
			update();

			accumulatedTime -= updateTime;
		}
	}
}

void Engine::update()
{
	m_time.update();
	m_window.update();
	glfwPollEvents();
	m_currentScene->update();
}

void Engine::stop()
{
	m_window.close();
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

Time& Engine::time()
{
	return m_time;
}
