#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <stb_image.h>

// Audio openal

// Could use std::filesystem::path

#include <Engine/Window.hpp>

#include <Engine/Init.hpp>
#include <Json/Json.hpp>
#include <Utils/FileIo.hpp>

#include <fstream>
#include <Log/Log.hpp>

#include <Engine/Ecs/Component.hpp>
#include <Engine/Ecs/ComponentPool.hpp>
#include <Engine/Ecs/EntityManager.hpp>

struct Test : public Component
{

	Test() = default;

	Test(std::string name)
	{
		std::cout << name << ": default\n";
	}

	void print()
	{
		std::cout << "hello\n";
	}

	~Test()
	{
		std::cout << "destructor\n";
	}

	Test(const Test&)
	{
		std::cout << "copy\n";
	}

	Test(Test&&) noexcept
	{
		std::cout << "move\n";
	}
};

#include <Utils/UninitializedArray.hpp>

// https://gamedev.stackexchange.com/questions/32187/when-and-why-to-use-console-logging

int main(int argc, char* argv[])
{
	EntityManager manager;
	std::cout << "Test id: " << getComponentId<Test>() << '\n';

	Entity& a = manager.addEntity();
	manager.entityAddComponent<Test>(a, "hello");

	Entity& b = manager.addEntity();
	manager.entityAddComponent<Test>(b, "goodbye");

	manager.removeDeadEntities();

	auto& pool = manager.getComponentPool<Test>();

	for (Test& t : pool)
	{
		t.print();
	}

	a.isAlive = false;
	//b.isAlive = false;

	manager.removeDeadEntities();

	manager.entityRemoveComponent<Test>(b);

	for (Test& t : pool)
	{
		t.print();
	}




	// Have to create the window between glfw and opengl init
	initGlfw();

	Json::Value json = jsonFromFile("../../../src/Config/window.json");
	Window window = [json]() 
	{
		try
		{
			return Window(json["width"].getIntNumber(), json["height"].getIntNumber(), "voxel-game");
		}
		catch (const Json::Value::InvalidTypeAccess&)
		{
			LOG_FATAL("invalid data in config file");
		}
	}();
	initOpenGl();

	while (window.shouldClose() == false)
	{
		if (glfwGetKey(window.handle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			window.close();

		window.swapBufers();
		glfwPollEvents();
	}

	glfwTerminate();
}