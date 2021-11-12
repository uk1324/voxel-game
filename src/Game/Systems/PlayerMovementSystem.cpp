#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Math/Angles.hpp>

#include <iostream>

void PlayerMovementSystem::update(Scene& scene, Entity& player)
{
	if (scene.input.isButtonHeld("forward"))
	{
		player.getComponent<Position>().value.z += 1.0f * scene.time.deltaTime();
	}

	if (scene.input.isButtonHeld("back"))
	{
		player.getComponent<Position>().value.z -= 1.0f * scene.time.deltaTime();
	}

	if (scene.input.isButtonHeld("left"))
	{
		Quaternion rotation = (Quaternion(degToRad(90.0f) * scene.time.deltaTime(), Vec3(0, 1, 0))).normalized();
		player.getComponent<Rotation>().value *= rotation;
	}

	if (scene.input.isButtonHeld("right"))
	{
		Quaternion rotation = (Quaternion(degToRad(-90.0f) * scene.time.deltaTime(), Vec3(0, 1, 0))).normalized();
		player.getComponent<Rotation>().value *= rotation;
	}

	std::cout << player.getComponent<Rotation>().value << '\n';

}
