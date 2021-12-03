#include <Engine/Engine.hpp>
#include <Engine/Window.hpp>
#include <Utils/Assertions.hpp>
#include <Log/Log.hpp>
#include <Engine/Graphics/GraphicsPrimitives.hpp>

Engine::Engine(int updatesPerSecond, Window& window)
	: m_currentScene(nullptr)
	, m_updatesPerSecond(updatesPerSecond)
	, m_window(window)
{}

void Engine::run(std::unique_ptr<Scene>&& startingScene)
{
	changeScene(std::move(startingScene));

	constexpr double FRAME_TIME_CAP = 2.0;

	double lastFrameStartTime = Time::currentTime();
	double frameStartTime;
	double frameTime;

	double accumulatedTime = 0.0;
	const double updateTime = 1.0 / m_updatesPerSecond;

	while (m_window.shouldClose() == false)
	{
		frameStartTime = Time::currentTime();
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
	m_currentScene->input.update();
	m_currentScene->update();
	m_window.update();
}

void Engine::stop()
{
	m_window.close();
}

void Engine::changeScene(std::unique_ptr<Scene>&& scene)
{
	m_currentScene = std::move(scene);
}

Window Engine::init(int windowWidth, int windowHeight, std::string_view windowTitle)
{
	// Before you can create a window you have to initialize GLFW and before you can initialize OpenGL you have to create a window.
	if (s_isInitialized == false)
	{
		initGlfw();
	}

	Window window = Window(windowWidth, windowHeight, windowTitle);

	if (s_isInitialized == false)
	{
		initOpenGl();

		GraphicsPrimitives::init();

		s_isInitialized = true;
	}

	return window;
}

void Engine::terminate()
{
	glfwTerminate();
}

void Engine::initGlfw()
{
	if (glfwInit() == GLFW_FALSE)
	{
		LOG_FATAL("failed to initialize GLFW");
	}
}

void Engine::initOpenGl()
{
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
	{
		LOG_FATAL("failed to initialize OpenGL");
	}
}

bool Engine::s_isInitialized = false;

Time& Engine::time()
{
	return m_time;
}

Window& Engine::window()
{
	return m_window;
}
