#pragma once

#include <Engine/Scene.hpp>

// Maybe store this in a file with PlayerMovementComponent

class PlayerMovementSystem
{
public:
	PlayerMovementSystem() = default;

	void update(Scene& scene, Entity& player);
};