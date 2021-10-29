#pragma once

#include <Engine/Ecs/EntityManager.hpp>

#include <string_view>
#include <memory>

class Engine;

class Scene
{
public:
	Scene(Engine& engine);
	virtual ~Scene() {};
	
	virtual void update() = 0;

	void changeScene(std::string_view name);

public:
	Engine& engine;
	EntityManager entityManager;
};