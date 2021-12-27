#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Game/Systems/PlayerMovementComponent.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/PhysicsSystem.hpp>
#include <Math/Angles.hpp>

void PlayerMovementSystem::registerActions(InputManager& inputManager)
{
	inputManager.registerKeyboardButton("forward", KeyCode::W);
	inputManager.registerKeyboardButton("back", KeyCode::S);
	inputManager.registerKeyboardButton("left", KeyCode::A);
	inputManager.registerKeyboardButton("right", KeyCode::D);
		 
	inputManager.registerKeyboardButton("jump", KeyCode::Space);
	inputManager.registerKeyboardButton("crouch", KeyCode::LeftShift);
}

void PlayerMovementSystem::update(Scene& scene, Entity& player)
{
	EntityManager& entityManager = scene.entityManager;
	Vec3& playerPosition = entityManager.entityGetComponent<Position>(player).value;
	Vec3& playerVel = entityManager.entityGetComponent<PhysicsVelocity>(player).value;
	Quat& playerRotation = entityManager.entityGetComponent<Rotation>(player).value;
	PlayerMovementComponent& playerMovement = entityManager.entityGetComponent<PlayerMovementComponent>(player);

	Vec2 offset = scene.input.mousePos() - scene.input.lastMousePos();

	playerMovement.rotationX -= offset.x * rotationSpeed * scene.time.deltaTime();
	playerMovement.rotationY += offset.y * rotationSpeed * scene.time.deltaTime();

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

	Vec3 movementDirection;

	if (scene.input.isButtonHeld("left"))
	{
		movementDirection += Vec3::left;
	}

	if (scene.input.isButtonHeld("right"))
	{
		movementDirection += Vec3::right;
	}

	if (scene.input.isButtonHeld("forward"))
	{
		movementDirection += Vec3::forward;
	}

	if (scene.input.isButtonHeld("back"))
	{
		movementDirection += Vec3::back;
	}

	//if (scene.input.isButtonHeld("jump"))
	//{
	//	movementDirection += Vec3::up;
	//}

	if (scene.input.isButtonHeld("crouch"))
	{
		movementDirection += Vec3::down;
	}

	movementDirection.normalize();
	const bool isEntityGrounded = entityManager.entityGetComponent<Grounded>(player).value;
	if (scene.input.isButtonDown("jump") && isEntityGrounded)
	{
		movementDirection += Vec3::up * 20;
	}

	playerVel += (rotationX * movementDirection) * walkSpeed * scene.time.deltaTime();

}
