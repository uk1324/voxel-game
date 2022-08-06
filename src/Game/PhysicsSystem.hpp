#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Time.hpp>
#include <Game/Blocks/ChunkSystem.hpp>

struct PhysicsComponent
{
	Vec3 velocity;
	Opt<BlockType> liquidCollisionBlockType;
};

// Maybe store this as min and max and then add the position to it. Also inherit from Aabb
struct PhysicsAabbCollider
{
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
	static Opt<BlockType> aabbVsLiquidCollision(const ChunkSystem& chunkSystem, const Vec3& pos, const Vec3& halfColliderSize);
	static bool isBlockVsAabbCollision(const Vec3& blockPos, const Vec3& pos, const Vec3& halfSize);
	Vec3 particleUpdatePosition(const ChunkSystem& chunkSystem, Vec3& pos, Vec3& vel);

private:
	float gravity = 0.5f;
	float airFriction = 0.98f;
	float groundFriction = 0.85f;
};