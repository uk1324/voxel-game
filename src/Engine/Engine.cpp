#include "Engine.hpp"
#include "Engine.hpp"
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

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	Window window = Window(windowWidth, windowHeight, windowTitle);

	if (s_isInitialized == false)
	{
		initOpenGl();

		Gfx::Primitives::init();

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
	glfwSetErrorCallback(glfwErrorCallback);
}

void Engine::initOpenGl()
{
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
	{
		LOG_FATAL("failed to initialize OpenGL");
	}


	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openGlErrorCallback, nullptr);
	}
	else
	{
		LOG_ERROR("failed to initialize debug output");
	}
}

void Engine::glfwErrorCallback(int errorCode, const char* errorMessage)
{
	LOG_FATAL("glfw error %d %s", errorCode, errorMessage);
}

void Engine::openGlErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::string errorMessage = "source: ";

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             errorMessage += "api"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   errorMessage += "window system"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: errorMessage += "shader compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     errorMessage += "third party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     errorMessage += "application"; break;
		case GL_DEBUG_SOURCE_OTHER:           errorMessage += "other"; break;
	}

	errorMessage += " type: ";

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               errorMessage += "error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: errorMessage += "deprecated behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  errorMessage += "undefined behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         errorMessage += "portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         errorMessage += "performance"; break;
		case GL_DEBUG_TYPE_MARKER:              errorMessage += "marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          errorMessage += "push group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           errorMessage += "pop group"; break;
		case GL_DEBUG_TYPE_OTHER:               errorMessage += "other"; break;
	}

	errorMessage += " severity: ";

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         errorMessage += "high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       errorMessage += "medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          errorMessage += "low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: errorMessage += "notification"; break;
	} 
	errorMessage += " message: ";
	errorMessage += message;
	LOG_FATAL("OpenGL error %s", errorMessage.c_str());
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
