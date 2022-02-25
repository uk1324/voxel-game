#pragma once

#include <Engine/Ecs/EntityManager.hpp>
#include <Game/PhysicsSystem.hpp>
#include <Game/Item/ItemStack.hpp>

struct ItemComponent
{
	ItemStack item;
	float spawnTime;
};

class EntitySystem
{
public:
	void update(EntityManager& entites);

	void spawnZombie(const Vec3& position, EntityManager& entityManager);

	static Entity spawnItemEntity(EntityManager& entityManager, const Vec3& position, ItemStack item, const Vec3& velocity = Vec3(0.0f));

private:

};