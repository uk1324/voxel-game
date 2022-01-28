#include <Engine/Engine.hpp>
#include <Engine/Window.hpp>
#include <Utils/Assertions.hpp>
#include <Log/Log.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

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
			m_time.update(updateTime);
			update();

			accumulatedTime -= updateTime;
		}
	}
}

void Engine::update()
{
	m_currentScene->input.update();
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplGlfw_NewFrame();
	//ImGui::NewFrame();
	m_currentScene->update();
 //	ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	m_window.update();
	m_currentScene->entityManager.update();
}

void Engine::stop()
{
	m_window.close();
}

void Engine::changeScene(std::unique_ptr<Scene> scene)
{
	m_currentScene = std::move(scene);
	// TODO: Fix this. Don't know why but I need to reregister the callbacks here even though they are registed in the constructor.
	glfwSetKeyCallback(m_window.handle(), InputManager::keyboardCallback);
	glfwSetCursorPosCallback(m_window.handle(), InputManager::mouseMoveCallback);
	glfwSetMouseButtonCallback(m_window.handle(), InputManager::mouseButtonCallback);
	glfwSetScrollCallback(m_window.handle(), InputManager::mouseScrollCallback);
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
		initImGui(window);

		s_isInitialized = true;
	}

	return window;
}

void Engine::terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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

void Engine::initImGui(Window& window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.handle(), true);
	ImGui_ImplOpenGL3_Init("#version 430");
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

	// Fragment shader recompiled due to state change.
	if (id == 2)
		return;

	// glLineWidth depracated.
	if (id == 7)
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
