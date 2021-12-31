#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/PhysicsSystem.hpp>
#include <Math/Angles.hpp>

#include <imgui.h>

PlayerMovementSystem::PlayerMovementSystem(Scene& scene)
{
	scene.entityManager.registerComponent<PlayerMovementComponent>();

	scene.input.registerKeyboardButton("forward", KeyCode::W);
	scene.input.registerKeyboardButton("back", KeyCode::S);
	scene.input.registerKeyboardButton("left", KeyCode::A);
	scene.input.registerKeyboardButton("right", KeyCode::D);

	scene.input.registerKeyboardButton("jump", KeyCode::Space);
	scene.input.registerKeyboardButton("crouch", KeyCode::LeftShift);
}

void PlayerMovementSystem::update(Scene& scene, Entity& player)
{
	EntityManager& entityManager = scene.entityManager;
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

	//if (scene.input.isButtonHeld("crouch"))
	//{
	//	movementDirection += Vec3::down;
	//}

	ImGui::Begin("jump");
	ImGui::SliderFloat("jump force", &jumpForce, 0, 100);
	ImGui::End();

	movementDirection.normalize();

	const bool isEntityGrounded = entityManager.entityGetComponent<Grounded>(player).value;

	if (isEntityGrounded)
	{
		playerVel += (rotationX * movementDirection) * walkSpeed * scene.time.deltaTime();
	}
	else
	{
		playerVel += (rotationX * movementDirection) * 0.1 * scene.time.deltaTime();
		//Vec3 speed = (rotationX * movementDirection) * walkSpeed * scene.time.deltaTime();
		//Vec3 velocity = playerVel.normalized();
		//velocity.y = 0.0f;
		//float d = dot(speed.normalized(), velocity.normalized());
		//if (d < 0)
		//	d = 0;
		//speed *= d;
		//playerVel += speed;
	}

	if (scene.input.isButtonHeld("jump"))
	{
		jumpPressedTime = Time::currentTime();
	}

	if (scene.input.isButtonDown("test"))
	{
		playerVel += (rotationX * movementDirection) * 10 * scene.time.deltaTime();
	}

	if (isEntityGrounded && Time::currentTime() - jumpPressedTime < 0.2 && Time::currentTime() - lastJump > 0.5)
	{
		//lastJump = Time::currentTime();

		if (movementDirection.lengthSquared() != 0)
			playerVel *= 1.2;
		playerVel += Vec3::up * jumpForce * scene.time.deltaTime();
	}

	ImGui::Begin("velocity");
	ImGui::Text("%f", Vec2(playerVel.x, playerVel.z).length());
	ImGui::End();


}
