#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Time.hpp>
#include <Game/Blocks/ChunkSystem.hpp>

struct PhysicsVelocity
{
public:
	Vec3 value;
};

// Maybe store this as min and max and then add the position to it. Also inherit from Aabb
struct PhysicsAabbCollider
{
public:
	Vec3 centerOffset;
	Vec3 halfSize;
};

struct Grounded
{
	bool value;
};

class PhysicsSystem
{
public:
	struct RaycastHit
	{
		Entity entity;
		float time;
	};

public:
	PhysicsSystem(Scene& scene);

	void update(const Time& time, EntityManager& entityManager, const ChunkSystem& chunkSystem);

	static Opt<RaycastHit> raycast(Entity entityToExclude, const Vec3& rayStart, const Vec3& rayEnd, const EntityManager& entityManager);

private:

	struct TerrainCollision
	{
		float entryTime;
		Vec3 normal;
	};

	static TerrainCollision aabbVsTerrainCollision(const ChunkSystem& chunkSystem, const Vec3& pos, const Vec3& halfSize, Vec3 vel);
	static bool isBlockVsAabbCollision(const Vec3& blockPos, const Vec3& pos, const Vec3& halfSize);

private:
	float gravity = 0.5;
};