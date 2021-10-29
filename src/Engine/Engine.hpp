#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Window.hpp>
#include <Engine/Time.hpp>

#include <string_view>
#include <unordered_map>
#include <memory>

class Engine
{

public:
	Engine(int updatesPerSecond, int windowWidth, int windowHeight, std::string_view windowTitle);
	~Engine();

	void run(std::string_view startingScene);
	void update();
	void stop();

	void addScene(std::string_view name, std::unique_ptr<Scene>&& scene);
	void changeScene(std::string_view name);

	Time& time();

private:
	Window m_window;

	bool m_isRunning;

	Scene* m_currentScene;
	std::unordered_map<std::string_view, std::unique_ptr<Scene>> m_scenes;

	Time m_time;
	int m_updatesPerSecond;
};