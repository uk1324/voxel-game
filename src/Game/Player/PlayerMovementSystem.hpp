#pragma once

#include <Engine/Scene.hpp>
#include <Game/PhysicsSystem.hpp>

struct PlayerMovementComponent
{
public:
	// I wanted to implement the camera using only quaternions but I couldn't find a good way to constrain a quaternion in a range
	// to prevent 360 rotations when looking up.
	float rotationX;
	float rotationY;
};

class PlayerMovementSystem
{
public:
	PlayerMovementSystem(Scene& scene);

	void update(Entity& player, const InputManager& input, const Time& time, EntityManager& entityManager);

public:
	float rotationSpeed = 0.2f;
	float walkSpeed = 1.0f;
	float airSpeedMultiplier = 0.1f;
	float lastJump = 0.0f;
	float airSpeed = 0.8f;
	float jumpForce = 0.17f;
	float jumpVelMultiplier = 1.2;

	double m_jumpPressedTime;
	double m_lastJumpTime;
};