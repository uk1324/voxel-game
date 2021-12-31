#pragma once

#include <Engine/Scene.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Time.hpp>
#include <Game/Blocks/ChunkSystem.hpp>
//#include <>

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

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

class PhysicsSystem
{
public:
	PhysicsSystem(Scene& scene);

	void update(const Time& time, EntityManager& entityManager, const ChunkSystem& chunkSystem);

private:
	void applyGravity(EntityManager& entityManager);

	struct TerrainCollision
	{
		float entryTime;
		Vec3 normal;
	};

	static TerrainCollision aabbVsTerrainCollision(const ChunkSystem& chunkSystem, const Vec3& pos, const Vec3& size, Vec3 vel);

	static bool isBlockVsAabbCollision(const Vec3& blockPos, const Vec3& pos, const Vec3& halfSize);

private:
	float gravity = 0.5;
};