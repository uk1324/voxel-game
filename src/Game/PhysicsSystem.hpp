#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Game/Blocks/ChunkSystem.hpp>

struct PhysicsVelocity
{
public:
	Vec3 value;
};

struct PhysicsAabbCollider
{
public:
	Vec3 centerOffset;
	Vec3 size;
};

struct Grounded
{
	bool value;
};

class PhysicsSystem
{
public:
	PhysicsSystem(Scene& scene);

	void update(EntityManager& entityManager, const ChunkSystem& chunkSystem);

private:
	void applyGravity(EntityManager& entityManager);
};