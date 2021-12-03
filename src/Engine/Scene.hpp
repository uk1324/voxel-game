#pragma once

#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Input/InputManager.hpp>
#include <Engine/Time.hpp>

#include <string_view>
#include <memory>

class Engine;

class Scene
{
public:
	Scene(Engine& engine);
	virtual ~Scene() {};
	
	virtual void update() = 0;

public:
	Engine& engine;
	EntityManager entityManager;
	InputManager input;
	Time& time;
};