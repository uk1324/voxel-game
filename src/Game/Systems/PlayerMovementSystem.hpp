#pragma once

#include <Engine/Scene.hpp>
#include <Game/PhysicsSystem.hpp>

struct PlayerMovementComponent
{
public:
	// I wanted to implement the camera using only quaternions but I couldn't find a good way to constrain a quaternion in a range.
	float rotationX;
	float rotationY;
};

class PlayerMovementSystem
{
public:
	PlayerMovementSystem(Scene& scene);

	void update(Scene& scene, Entity& player);

public:
	float rotationSpeed = 0.2f;
	float walkSpeed = 1.0f;
	float airSpeed = 0.8f;
	float jumpForce = 10.0f;
	double jumpPressedTime;
};