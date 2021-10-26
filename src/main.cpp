#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <stb_image.h>

// Audio openal

#include <Json/JsonValue.hpp>
#include <Json/JsonPrinter.hpp>
#include <Json/JsonParser.hpp>

#include <fstream>

int main(int argc, char* argv[])
{

	//Json::Value json = {
	//	{"pi", 3.141},
	//	{"happy", true},
	//	{"name", "Ni\ne\tls\r"},
	//	{"nothing", nullptr},
	//	{"answer", {
	//		{"everything", 42}
	//	}},
	//	{"list", {1, 0, 2}},
	//	{"object", {
	//		{"currency", "USD"},
	//		{"value", 42.99}
	//	}}
	//};
	Json::Value json = Json::parse("{\"glossary\":{\"title\":\"example glossary\",\"GlossDiv\":{\"title\":\"S\",\"GlossList\":{\"GlossEntry\":{\"ID\":\"SGML\",\"SortAs\":\"SGML\",\"GlossTerm\":\"Standard Generalized Markup Language\",\"Acronym\":\"SGML\",\"Abbrev\":\"ISO 8879:1986\",\"GlossDef\":{\"para\":\"A meta-markup language, used to create markup languages such as DocBook.\",\"GlossSeeAlso\":[\"GML\",\"XML\"]},\"GlossSee\":\"markup\"}}}}}   ");
	Json::prettyPrint(std::cout, json);



	//glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	//GLFWwindow* window = glfwCreateWindow(800, 600, "opengl", NULL, NULL);
	//if (window == nullptr)
	//{
	//	std::cout << "Failed to create GLFW window\n";
	//	glfwTerminate();
	//	exit(EXIT_FAILURE);
	//}

	//glfwMakeContextCurrent(window);

	//if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == false)
	//{
	//	std::cout << "Failed to initialize GLAD\n";
	//	exit(EXIT_FAILURE);
	//}

	//while (glfwWindowShouldClose(window) == false)
	//{
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();
	//}

	//glfwTerminate();

	//std::map<int, int> map = {
	//	{ 1, 1 },
	//	{ 2, 1 },
	//	{ 3, 1 },
	//	{ 4, 1 },
	//	{ 5, 1 }
	//};

	//for (auto [one, two] : map)
	//{
	//	std::cout << one << " " << two << '\n';
	//}

	//std::cout << "tset";
}