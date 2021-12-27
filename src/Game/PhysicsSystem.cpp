#include <Game/PhysicsSystem.hpp>
#include <Game/Components/Position.hpp>


#include <Game/Rendering/RenderingSystem.hpp>

PhysicsSystem::PhysicsSystem(Scene& scene)
{
	scene.entityManager.registerComponent<PhysicsVelocity>();
	scene.entityManager.registerComponent<PhysicsAabbCollider>();
	scene.entityManager.registerComponent<Grounded>();
}

#include <iostream>

void PhysicsSystem::update(EntityManager& entityManager, const ChunkSystem& chunkSystem)
{
	for (auto& [entity, velocityComponent] : entityManager.getComponents<PhysicsVelocity>())
	{
		Vec3& entityVel = velocityComponent.value;
		Vec3& entityPos = entityManager.entityGetComponent<Position>(entity).value;
		bool& isEntityGrounded = entityManager.entityGetComponent<Grounded>(entity).value;
		const PhysicsAabbCollider& collider = entityManager.entityGetComponent<PhysicsAabbCollider>(entity);

		const Vec3 colliderPos = entityPos + collider.centerOffset;
		const Vec3 halfSize = collider.size / 2.0f;

		std::vector<Vec3> lastCollisions;

		auto isCollision = [&lastCollisions, &halfSize, &chunkSystem](Vec3 pos) -> bool
		{
			const Vec3 start = (pos - halfSize).applied(floor);
			const Vec3 end = (pos + halfSize).applied(ceil);

			bool yes = false;

			for (float z = start.z; z < end.z; z += Block::SIZE)
			{
				for (float y = start.y; y < end.y; y += Block::SIZE)
				{
					for (float x = start.x; x < end.x; x += Block::SIZE)
					{
						if (chunkSystem.get(x, y, z).isSolid())
						{
							lastCollisions.push_back(Vec3(x, y, z));
							yes = true;
						}
					}
				}
			}
			return yes;
		};

		entityVel += Vec3(0, -0.01, 0);
		entityVel *= 0.85;


		const Vec3 start = (colliderPos - halfSize).applied(floor);
		const Vec3 end = (colliderPos + halfSize).applied(ceil);

		for (float z = start.z; z < end.z; z += Block::SIZE)
		{
			for (float y = start.y; y < end.y; y += Block::SIZE)
			{
				for (float x = start.x; x < end.x; x += Block::SIZE)
				{
					const Vec3 voxelPos(x, y, z);
					Debug::drawCube(voxelPos + Vec3(Block::SIZE / 2.0f), Vec3(1, 1, 1));

					if (chunkSystem.get(x, y, z).isSolid())
					{
						const Vec3 positiveDistance = Vec3((entityVel - halfSize) - (Vec3(x, y, z) + Vec3(Block::SIZE))).applied(abs);
						const Vec3 negativeDistance = Vec3((entityVel + halfSize) - Vec3(x, y, z)).applied(abs);

						enum class CollisionSide
						{
							PositiveX,
							PositiveY,
							PositiveZ,
							NegativeX,
							NegativeY,
							NegativeZ,
							None,
						};

						const std::array<float, 6> distances = {
							positiveDistance.x,
							positiveDistance.y,
							positiveDistance.z,
							negativeDistance.x,
							negativeDistance.y,
							negativeDistance.z
						};

						CollisionSide closestSide = CollisionSide::None;
						float distanceToClosestSide = std::numeric_limits<float>::infinity();

						for (size_t i = 0; i < distances.size(); i++)
						{
							if (distances[i] < distanceToClosestSide)
							{
								closestSide = static_cast<CollisionSide>(i);
								distanceToClosestSide = distances[i];
							}
						}

						switch (closestSide)
						{
							case CollisionSide::PositiveX:
								entityPos.x = voxelPos.x + Block::SIZE + halfSize.x - collider.centerOffset.x;
								entityVel.x = 0;
								break;

							case CollisionSide::PositiveY:
								entityPos.y = voxelPos.y + Block::SIZE + halfSize.y - collider.centerOffset.y;
								entityVel.y = 0;
								break;

							case CollisionSide::PositiveZ:
								entityPos.z = voxelPos.z + Block::SIZE + halfSize.z - collider.centerOffset.z;
								entityVel.z = 0;
								break;

							case CollisionSide::NegativeX:
								entityPos.x = voxelPos.x - halfSize.x - collider.centerOffset.x;
								entityVel.x = 0;
								break;

							case CollisionSide::NegativeY:
								entityPos.y = voxelPos.y - halfSize.y - collider.centerOffset.y;
								entityVel.y = 0;
								break;

							case CollisionSide::NegativeZ:
								entityPos.z = voxelPos.z - halfSize.z - collider.centerOffset.z;
								entityVel.z = 0;
								break;
						}
					}
				}
			}
		}

		Vec3 newColliderPos;
		
		newColliderPos = colliderPos + Vec3(entityVel.x, 0, 0);
		if (isCollision(newColliderPos) == false)
			entityPos.x += entityVel.x;
		else
			entityVel.x = 0;

		newColliderPos = colliderPos + Vec3(0, entityVel.y, 0);
		if (isCollision(newColliderPos) == false)
		{
			entityPos.y += entityVel.y;
			isEntityGrounded = false;
		}
		else
		{
			entityVel.y = 0;
			isEntityGrounded = true;
		}

		newColliderPos = colliderPos + Vec3(0, 0, entityVel.z);
		if (isCollision(newColliderPos) == false)
			entityPos.z += entityVel.z;
		else
			entityVel.z = 0;

		Debug::drawCube(entityPos + collider.centerOffset, collider.size, Vec3(0, 1, 0));
	}

	//for (const auto& [entity, collider] : entityManager.getComponents<PhysicsAabbCollider>())
	//{
	//	Vec3& entityPos = entityManager.entityGetComponent<Position>(entity).value;
	//	Vec3& entityVel = entityManager.entityGetComponent<PhysicsVelocity>(entity).value;

	//	const Vec3 pos = entityPos + collider.centerOffset;
	//	const Vec3 halfSize = collider.size / 2.0f;

	//	const Vec3 start = (pos - halfSize).applied(floor);
	//	const Vec3 end = (pos + halfSize).applied(ceil);

	//	enum class CollisionSide
	//	{
	//		PositiveX,
	//		PositiveY,
	//		PositiveZ,
	//		NegativeX,
	//		NegativeY,
	//		NegativeZ,
	//		None,
	//	};

	//	Vec3 collisionAreaSize = (end - start).applied(ceil);

	//	struct Collision
	//	{
	//		Vec3 position;
	//		CollisionSide side;
	//		bool shouldBeResolved;
	//	};

	//	std::vector<Collision> collisions;

	//	for (float z = start.z; z < end.z; z += Block::SIZE)
	//	{
	//		for (float y = start.y; y < end.y; y += Block::SIZE)
	//		{
	//			for (float x = start.x; x < end.x; x += Block::SIZE)
	//			{
	//				// Could just iterate over it
	//				const Vec3 voxelPos(x, y, z);
	//				if (chunkSystem.get(x, y, z).isSolid())
	//				{
	//					const Vec3 positiveDistance = Vec3((pos - halfSize) - (Vec3(x, y, z) + Vec3(Block::SIZE))).applied(abs);
	//					const Vec3 negativeDistance = Vec3((pos + halfSize) - Vec3(x, y, z)).applied(abs); 

	//					const std::array<float, 6> distances = {
	//						positiveDistance.x,
	//						positiveDistance.y,
	//						positiveDistance.z,
	//						negativeDistance.x,
	//						negativeDistance.y,
	//						negativeDistance.z
	//					};

	//					CollisionSide closestSide = CollisionSide::None;
	//					float distanceToClosestSide = std::numeric_limits<float>::infinity();

	//					const std::vector<bool (*)(const ChunkSystem&, float, float, float)> a = {
	//						[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x + 1, y, z) == BlockType::Air; },
	//						[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x, y + 1, z) == BlockType::Air; },
	//						[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x, y, z + 1) == BlockType::Air; },
	//						[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x - 1, y, z) == BlockType::Air; },
	//						[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x, y - 1, z) == BlockType::Air; },
	//						[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x, y, z - 1) == BlockType::Air; },
	//					};

	//					for (size_t i = 0; i < distances.size(); i++)
	//					{
	//						if (a[i](chunkSystem, x, y, z) == false)
	//							continue;
	//						if (distances[i] < distanceToClosestSide)
	//						{
	//							closestSide = static_cast<CollisionSide>(i);
	//							distanceToClosestSide = distances[i];
	//						}
	//					}

	//					//if (a[static_cast<int>(closestSide)](chunkSystem, x, y, z) )
	//					if (closestSide != CollisionSide::None)
	//					{
	//						collisions.push_back(Collision{ voxelPos, closestSide, true });
	//					}

	//				}
	//			}
	//		}
	//	}

	//	auto resolveCollision = [&halfSize](Vec3& pos, const Vec3& voxelPos, CollisionSide side) -> void
	//	{
	//		switch (side)
	//		{
	//		case CollisionSide::PositiveX:
	//			pos.x = voxelPos.x + Block::SIZE + halfSize.x;
	//			break;

	//		case CollisionSide::PositiveY:
	//			pos.y = voxelPos.y + Block::SIZE + halfSize.y;
	//			break;

	//		case CollisionSide::PositiveZ:
	//			pos.z = voxelPos.z + Block::SIZE + halfSize.z;
	//			break;

	//		case CollisionSide::NegativeX:
	//			pos.x = voxelPos.x - halfSize.x;
	//			break;

	//		case CollisionSide::NegativeY:
	//			pos.y = voxelPos.y - halfSize.y;
	//			break;

	//		case CollisionSide::NegativeZ:
	//			pos.z = voxelPos.z - halfSize.z;
	//			break;
	//		}
	//	};

	//	auto isCollision = [&chunkSystem, &halfSize](const Vec3& pos, const Vec3& voxelPos) -> bool
	//	{
	//		return (pos.x - halfSize.x < voxelPos.x + Block::SIZE) && (pos.x + halfSize.x > voxelPos.x)
	//			&& (pos.y - halfSize.y < voxelPos.y + Block::SIZE) && (pos.y + halfSize.y > voxelPos.y)
	//			&& (pos.z - halfSize.z < voxelPos.z + Block::SIZE) && (pos.z + halfSize.z > voxelPos.z);
	//	};

	//	for (size_t i = 0; i < collisions.size(); i++)
	//	{
	//		Vec3 testPos = pos;

	//		if (collisions[i].shouldBeResolved == false)
	//			continue;

	//		resolveCollision(testPos, collisions[i].position, collisions[i].side);

	//		for (size_t j = 0; j < i; j++)
	//		{
	//			//if (collisions[j].shouldBeResolved == false)
	//			//	continue;

	//			if (isCollision(testPos, collisions[j].position) == false)
	//				collisions[j].shouldBeResolved = false;
	//			else
	//				collisions[j].shouldBeResolved = true;
	//		}

	//		for (size_t j = i + 1; j < collisions.size(); j++)
	//		{
	//			//if (collisions[j].shouldBeResolved == false)
	//			//	continue;

	//			if (isCollision(testPos, collisions[j].position) == false)
	//				collisions[j].shouldBeResolved = false;
	//			else
	//				collisions[j].shouldBeResolved = true;
	//		}
	//	}

	//	for (const Collision& collision : collisions)
	//	{
	//		if (collision.shouldBeResolved)
	//		{
	//			switch (collision.side)
	//			{
	//			case CollisionSide::PositiveX:
	//				Debug::drawCube(collision.position + Vec3(Block::SIZE, Block::SIZE / 2.0f, Block::SIZE / 2.0f) + Vec3(0.01), Vec3(0, 1, 1));
	//				entityPos.x = collision.position.x + Block::SIZE + halfSize.x - collider.centerOffset.x;
	//				entityVel.x = 0;
	//				break;

	//			case CollisionSide::PositiveY:
	//				Debug::drawCube(collision.position + Vec3(Block::SIZE / 2.0f, Block::SIZE, Block::SIZE / 2.0f) + Vec3(0.01), Vec3(1, 0, 1));
	//				entityPos.y = collision.position.y + Block::SIZE + halfSize.y - collider.centerOffset.y;
	//				entityVel.y = 0;
	//				break;

	//			case CollisionSide::PositiveZ:
	//				Debug::drawCube(collision.position + Vec3(Block::SIZE / 2.0f, Block::SIZE / 2.0f, Block::SIZE) + Vec3(0.01), Vec3(1, 1, 0));
	//				entityPos.z = collision.position.z + Block::SIZE + halfSize.z - collider.centerOffset.z;
	//				entityVel.z = 0;
	//				break;

	//			case CollisionSide::NegativeX:
	//				Debug::drawCube(collision.position + Vec3(0, Block::SIZE / 2.0f, Block::SIZE / 2.0f) + Vec3(0.01), Vec3(0, 1, 1));
	//				entityPos.x = collision.position.x - halfSize.x - collider.centerOffset.x;
	//				entityVel.x = 0;
	//				break;

	//			case CollisionSide::NegativeY:
	//				Debug::drawCube(collision.position + Vec3(Block::SIZE / 2.0f, 0, Block::SIZE / 2.0f) + Vec3(0.01), Vec3(1, 0, 1));
	//				entityPos.y = collision.position.y - halfSize.y - collider.centerOffset.y;
	//				entityVel.y = 0;
	//				break;

	//			case CollisionSide::NegativeZ:
	//				Debug::drawCube(collision.position + Vec3(Block::SIZE / 2.0f, Block::SIZE / 2.0f, 0 ) + Vec3(0.01), Vec3(1, 1, 0));
	//				entityPos.z = collision.position.z - halfSize.z - collider.centerOffset.z;
	//				entityVel.z = 0;
	//				break;
	//			}
	//		}
	//	}


	//	Debug::drawCube(entityPos + collider.centerOffset, collider.size, Vec3(0, 1, 0));
	//}
}

void PhysicsSystem::applyGravity(EntityManager& entityManager)
{
	//for (auto& [entity, velocityComponent] : entityManager.getComponents<PhysicsVelocity>())
	//{
	//	Vec3& vel = velocityComponent.value;
	//	Vec3& pos = entityManager.entityGetComponent<Position>(entity).value;

	//	vel += Vec3(0, -0.01, 0);
	//	vel *= 0.85;

	//	if ()

	//	pos += vel;
	//}
}
