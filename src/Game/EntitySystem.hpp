#pragma once

#include <Engine/Ecs/EntityManager.hpp>
#include <Game/PhysicsSystem.hpp>
#include <Game/Item/ItemStack.hpp>
#include <Game/Components/AnimatedModel.hpp>
#include <Game/Blocks/BlockSystem.hpp>

struct ItemComponent
{
	ItemStack item;
	float spawnTime;
};

struct ZombieComponent
{
	float start;
	bool idle;
	Quat newRotation;
	Quat oldRotation;
};

class EntitySystem
{
public:
	EntitySystem();
	void update(EntityManager& entites, const InputManager& input, const BlockSystem& blocks);

	void spawnZombie(const Vec3& position, EntityManager& entityManager);

	static Entity spawnItemEntity(EntityManager& entityManager, const Vec3& position, ItemStack item, const Vec3& velocity = Vec3(0.0f));

private:
	Model m_zombieModel;
};