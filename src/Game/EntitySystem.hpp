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

	static Entity spawnItemEntity(const Vec3& position, ItemStack item, EntityManager& entityManager);

private:

};