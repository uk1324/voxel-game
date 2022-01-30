#include <Game/Player/PlayerMovementSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/PhysicsSystem.hpp>
#include <Math/Angles.hpp>

PlayerMovementSystem::PlayerMovementSystem(Scene& scene)
	: m_jumpPressedTime(0.0)
	, m_lastJumpTime(0.0)
{
	scene.entityManager.registerComponent<PlayerMovementComponent>();
}

void PlayerMovementSystem::update(Entity& player, const InputManager& input, const Time& time, EntityManager& entityManager)
{
	Vec3& playerVel = entityManager.getComponent<PhysicsVelocity>(player).value;
	Quat& playerRotation = entityManager.getComponent<Rotation>(player).value;
	PlayerMovementComponent& playerMovement = entityManager.getComponent<PlayerMovementComponent>(player);

	Vec2 offset = input.mousePos() - input.lastMousePos();

	playerMovement.rotationX -= offset.x * rotationSpeed * time.deltaTime();
	playerMovement.rotationY += offset.y * rotationSpeed * time.deltaTime();

	if (playerMovement.rotationX > degToRad(360.0f))
	{
		playerMovement.rotationX -= degToRad(360.0f);
	}
	else if (playerMovement.rotationX < degToRad(-360.0f))
	{
		playerMovement.rotationX += degToRad(360.0f);
	}

	// Limit rotation up and down.
	// Using 89.50 so the Gram-Schmidt process doesn't fail (read Mat4::lookAt). 
	if (playerMovement.rotationY > degToRad(89.50f))
	{
		playerMovement.rotationY = degToRad(89.50f);
	}
	else if (playerMovement.rotationY < -degToRad(89.50f))
	{
		playerMovement.rotationY = -degToRad(89.50f);
	}

	Quat rotationX(playerMovement.rotationX, Vec3::yAxis);
	Quat rotationY(playerMovement.rotationY, Vec3::xAxis);
	playerRotation = rotationX * rotationY;

	Vec3 movementDirection(0.0f);
	if (input.isButtonHeld("left"))
		movementDirection += Vec3::left;
	if (input.isButtonHeld("right"))
		movementDirection += Vec3::right;
	if (input.isButtonHeld("forward"))
		movementDirection += Vec3::forward;
	if (input.isButtonHeld("back"))
		movementDirection += Vec3::back;

	/*playerVel = playerRotation * movementDirection * 0.5f;

	if (input.isButtonHeld("jump"))
		playerVel.y = 0.5;
	if (input.isButtonHeld("crouch"))
		playerVel.y = -0.5;*/

#if 1
	movementDirection.normalize();


	const bool isEntityGrounded = entityManager.getComponent<Grounded>(player).value;
	if (isEntityGrounded)
	{
		playerVel += (rotationX * movementDirection) * walkSpeed * time.deltaTime();
	}
	else
	{
		playerVel += (rotationX * movementDirection) * airSpeedMultiplier * time.deltaTime();
	}

	if (input.isButtonDown("test"))
	{
		playerVel *= 3.0f;
	}

	bool isJumpBuffered = true;
	if (input.isButtonDown("jump"))
	{
		isJumpBuffered = false;
	}

	if (input.isButtonHeld("jump"))
	{
		m_jumpPressedTime = Time::currentTime();
	}

	static constexpr double JUMP_BUFFER_THRESHOLD = 0.2;
	if (isEntityGrounded && ((Time::currentTime() - m_jumpPressedTime) < JUMP_BUFFER_THRESHOLD))
	{
		// Prevents really fast buffered jumping for example when hitting the top of the collider against the ceiling.
		static constexpr double INTERVAL_BETWEEN_BUFFERED_JUMPS = 0.3;
		if (isJumpBuffered && ((Time::currentTime() - m_lastJumpTime) < INTERVAL_BETWEEN_BUFFERED_JUMPS))
		{
			return;
		}
		if (movementDirection.lengthSquared() != 0.0f)
			playerVel *= jumpVelMultiplier;
		playerVel += Vec3::up * jumpForce;

		m_lastJumpTime = Time::currentTime();
	}

#endif // 0
}
