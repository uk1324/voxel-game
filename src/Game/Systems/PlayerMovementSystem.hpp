#pragma once

#include <Engine/Scene.hpp>
#include <Game/PhysicsSystem.hpp>

// Maybe store this in a file with PlayerMovementComponent

class PlayerMovementSystem
{
public:
	PlayerMovementSystem() = default;

	void registerActions(InputManager& inputManager);
	void update(Scene& scene, Entity& player);

public:
	float rotationSpeed = 0.2f;
	/*float walkSpeed = 4.0f;*/
	float walkSpeed = 1.f;
};