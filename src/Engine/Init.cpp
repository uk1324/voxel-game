#include <Engine/Init.hpp>
#include <Error/Error.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void initGlfw()
{
	if (glfwInit() == GLFW_FALSE)
	{
		panic("failed to initialize GLFW");
	}
}

void initOpenGl()
{
	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
	{
		panic("failed to initialize OpenGL");
	}
}