#include <Engine/Init.hpp>
#include <Log/Log.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void initGlfw()
{
	if (glfwInit() == GLFW_FALSE)
	{
		LOG_FATAL("failed to initialize GLFW");
	}
}

void initOpenGl()
{
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
	{
		LOG_FATAL("failed to initialize OpenGL");
	}
}