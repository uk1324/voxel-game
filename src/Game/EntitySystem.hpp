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

struct BlockParticleEmmiter
{
	struct Particle
	{
		Vec3 pos;
		Vec3 vel;
	};
	std::array<Particle, 10> particles;
	float spawnTime;
	BlockType blockType;
};

class EntitySystem
{
public:
	EntitySystem();
	void update(EntityManager& entites, const InputManager& input, const BlockSystem& blocks);

	void spawnZombie(const Vec3& position, EntityManager& entityManager);

	static void spawnBlockParticles(const Vec3& position, BlockType blockType, EntityManager& entityManager);
	static Entity spawnItemEntity(EntityManager& entityManager, const Vec3& position, ItemStack item, const Vec3& velocity = Vec3(0.0f));

private:
	Model m_zombieModel;
};