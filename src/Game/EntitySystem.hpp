#pragma once

#include <Engine/Ecs/EntityManager.hpp>
#include <Game/PhysicsSystem.hpp>

class EntitySystem
{
public:
	void update(EntityManager& entites);

	void spawnZombie(const Vec3& position, EntityManager& entityManager);

private:

};