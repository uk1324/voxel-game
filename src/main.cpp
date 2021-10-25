#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "opengl", NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
	{
		std::cout << "Failed to initialize GLAD\n";
		exit(EXIT_FAILURE);
	}

	while (glfwWindowShouldClose(window) == false)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	std::map<int, int> map = {
		{ 1, 1 },
		{ 2, 1 },
		{ 3, 1 },
		{ 4, 1 },
		{ 5, 1 }
	};

	for (auto [one, two] : map)
	{
		std::cout << one << " " << two << '\n';
	}

	std::cout << "tset";
}