#include <Game/PhysicsSystem.hpp>
#include <Game/Components/Position.hpp>

PhysicsSystem::PhysicsSystem(Scene& scene)
{
	scene.entityManager.registerComponent<PhysicsVelocity>();
	scene.entityManager.registerComponent<PhysicsAabbCollider>();
	scene.entityManager.registerComponent<Grounded>();
}

// Prevent from getting into unloaded chunks or stop player if is inside unloaded chunks idk.

#include <Game/Debug/Debug.hpp>

void PhysicsSystem::update(const Time& time, EntityManager& entityManager, const ChunkSystem& chunkSystem)
{
	for (auto& [entity, velocityComponent] : entityManager.getComponents<PhysicsVelocity>())
	{
		Vec3& entityVel = velocityComponent.value;
		Vec3& entityPos = entityManager.getComponent<Position>(entity).value;
		Opt<Grounded*> isEntityGrounded = entityManager.getOptComponent<Grounded>(entity);
		const PhysicsAabbCollider& collider = entityManager.getComponent<PhysicsAabbCollider>(entity);

		entityVel.y -= gravity * time.deltaTime();

		entityVel.y *= 0.98;

		if (isEntityGrounded.has_value())
		{
			if ((*isEntityGrounded)->value)
			{
				entityVel.x *= 0.85;
				entityVel.z *= 0.85;
			}
			else
			{
				entityVel.x *= 0.98;
				entityVel.z *= 0.98;
			}

			(*isEntityGrounded)->value = false;
		}

		Vec3 movement = entityVel;
		// Doing collsion 3 times so the sliding collision response on one axis doesn't allow to go through blocks.
		for (int i = 0; i < 3; i++)
		{
			TerrainCollision collision = aabbVsTerrainCollision(chunkSystem, entityPos + collider.centerOffset, collider.halfSize, entityVel);

			static constexpr float EPSILON = 0.01f;
			// Subtracting epsilon to move the player right before the collision.
			entityPos += movement * (collision.entryTime - EPSILON);

			float remainingTime = 1.0f - collision.entryTime;
			// If all movement was resolved.
			if (remainingTime <= 0.0f)
				break;

			movement *= remainingTime;

			if (collision.normal.y == 1.0f)
			{
				if (isEntityGrounded.has_value())
				{
					(*isEntityGrounded)->value = true;
				}
			}

			if (collision.normal.x != 0.0f)
			{
				entityVel.x = 0.0f;
				movement.x = 0.0f;
			}
			else if (collision.normal.y != 0.0f)
			{
				entityVel.y = 0.0f;
				movement.y = 0.0f;
			}
			else if (collision.normal.z != 0.0f)
			{
				entityVel.z = 0.0f;
				movement.z = 0.0f;
			}
		}
		
		if (Debug::shouldShowHitboxes)
		{
			Debug::drawCube(entityPos + collider.centerOffset, collider.halfSize * 2);
		}
	}
}

Opt<PhysicsSystem::RaycastHit> PhysicsSystem::raycast(Entity entityToExclude, const Vec3& rayStart, const Vec3& rayEnd, const EntityManager& entityManager)
{
	bool hit = false;
	float closestHitTime = std::numeric_limits<float>::infinity();
	Entity hitEntity;

	for (const auto& [entity, collider] : entityManager.getComponents<PhysicsAabbCollider>())
	{
		const Vec3 pos = entityManager.getComponent<Position>(entity).value + collider.centerOffset;
		if (entity != entityToExclude)
		{
			if ((rayStart.x > (pos.x - collider.halfSize.x)) && (rayStart.x < (pos.x + collider.halfSize.x))
				&& (rayStart.y > (pos.y - collider.halfSize.y)) && (rayStart.y < (pos.y + collider.halfSize.y))
				&& (rayStart.z > (pos.z - collider.halfSize.z)) && (rayStart.z < (pos.z + collider.halfSize.z)))
			{
				return RaycastHit{ entity, 0.0f };
			}

			Vec3 delta = rayEnd - rayStart;

			Vec3 entryDistance;
			Vec3 exitDistance;

			if (delta.x > 0)
			{
				entryDistance.x = (pos.x - collider.halfSize.x) - rayStart.x;
				exitDistance.x = (pos.x + collider.halfSize.x) - rayStart.x;
			}
			else
			{
				entryDistance.x = (pos.x + collider.halfSize.x) - rayStart.x;
				exitDistance.x =  (pos.x - collider.halfSize.x) - rayStart.x;
			}

			if (delta.y > 0)
			{
				entryDistance.y = (pos.y - collider.halfSize.y) - rayStart.y;
				exitDistance.y = (pos.y + collider.halfSize.y) - rayStart.y;
			}
			else
			{
				entryDistance.y = (pos.y + collider.halfSize.y) - rayStart.y;
				exitDistance.y = (pos.y - collider.halfSize.y) - rayStart.y;
			}

			if (delta.z > 0)
			{
				entryDistance.z = (pos.z - collider.halfSize.z) - rayStart.z;
				exitDistance.z = (pos.z + collider.halfSize.z) - rayStart.z;
			}
			else
			{
				entryDistance.z = (pos.z + collider.halfSize.z) - rayStart.z;
				exitDistance.z = (pos.z - collider.halfSize.z) - rayStart.z;
			}

			Vec3 entryTime;
			Vec3 exitTime;

			if (delta.x == 0.0f)
			{
				entryTime.x = -std::numeric_limits<float>::infinity();
				exitTime.x = std::numeric_limits<float>::infinity();
			}
			else
			{
				entryTime.x = entryDistance.x / delta.x;
				exitTime.x = exitDistance.x / delta.x;
			}

			if (delta.y == 0.0f)
			{
				entryTime.y = -std::numeric_limits<float>::infinity();
				exitTime.y = std::numeric_limits<float>::infinity();
			}
			else
			{
				entryTime.y = entryDistance.y / delta.y;
				exitTime.y = exitDistance.y / delta.y;
			}

			if (delta.z == 0.0f)
			{
				entryTime.z = -std::numeric_limits<float>::infinity();
				exitTime.z = std::numeric_limits<float>::infinity();
			}
			else
			{
				entryTime.z = entryDistance.z / delta.z;
				exitTime.z = exitDistance.z / delta.z;
			}

			// Finding max entry to find the most movement possible before collision on any axis.
			float entry = std::max(entryTime.x, std::max(entryTime.y, entryTime.z));
			float exit = std::min(exitTime.x, std::min(exitTime.y, exitTime.z));

			// No collision
			if ((entry > exit)
				|| ((entryTime.x < 0.0f) && (entryTime.y < 0.0f) && (entryTime.z < 0.0f))
				|| (entryTime.x > 1.0f)
				|| (entryTime.y > 1.0f)
				|| (entryTime.z > 1.0f)
				// Closer collision already found.
				|| (entry > closestHitTime))
			{
				continue;
			}
			else
			{
				if (entry < closestHitTime)
				{
					closestHitTime = entry;
					hit = true;
					hitEntity = entity;
				}
			}
		}
	}

	if (hit)
		return RaycastHit{ hitEntity, closestHitTime };
	return std::nullopt;
}

PhysicsSystem::TerrainCollision PhysicsSystem::aabbVsTerrainCollision(const ChunkSystem& chunkSystem, const Vec3& pos, const Vec3& halfSize, Vec3 vel)
{
	TerrainCollision result;
	result.entryTime = 1.0f;
	result.normal = Vec3(0.0f);

	Vec3 newPos = pos + vel;

	Vec3 collisionAreaMin;
	Vec3 collisionAreaMax;

	if (pos.x < newPos.x)
	{
		collisionAreaMin.x = pos.x;
		collisionAreaMax.x = newPos.x;
	}
	else
	{
		collisionAreaMin.x = newPos.x;
		collisionAreaMax.x = pos.x;
	}

	if (pos.y < newPos.y)
	{
		collisionAreaMin.y = pos.y;
		collisionAreaMax.y = newPos.y;
	}
	else
	{
		collisionAreaMin.y = newPos.y;
		collisionAreaMax.y = pos.y;
	}

	if (pos.z < newPos.z)
	{
		collisionAreaMin.z = pos.z;
		collisionAreaMax.z = newPos.z;
	}
	else
	{
		collisionAreaMin.z = newPos.z;
		collisionAreaMax.z = pos.z;
	}

	collisionAreaMin -= halfSize;
	collisionAreaMax += halfSize;
	collisionAreaMin.apply(floor);
	collisionAreaMax.apply(ceil);

	for (float z = collisionAreaMin.z; z < collisionAreaMax.z; z++)
	{
		for (float y = collisionAreaMin.y; y < collisionAreaMax.y; y++)
		{
			for (float x = collisionAreaMin.x; x < collisionAreaMax.x; x++)
			{
				auto optBlock = chunkSystem.tryGetBlock(Vec3I(x, y, z));
				if (optBlock.has_value() && optBlock->type != BlockType::Air && chunkSystem.blockData[optBlock->type].isSolid
					// Disable collision if collider is already inside the block.
					&& (isBlockVsAabbCollision(Vec3(x, y, z), pos, halfSize) == false))
				{
					Vec3 entryDistance;
					Vec3 exitDistance;

					if (vel.x > 0)
					{
						entryDistance.x = x - (pos.x + halfSize.x);
						exitDistance.x = (x + Block::SIZE) - (pos.x - halfSize.x);
					}
					else
					{
						entryDistance.x = (x + Block::SIZE) - (pos.x - halfSize.x);
						exitDistance.x = x - (pos.x + halfSize.x);
					}

					if (vel.y > 0)
					{
						entryDistance.y = y - (pos.y + halfSize.y);
						exitDistance.y = (y + Block::SIZE) - (pos.y - halfSize.y);
					}
					else
					{
						entryDistance.y = (y + Block::SIZE) - (pos.y - halfSize.y);
						exitDistance.y = y - (pos.y + halfSize.y);
					}

					if (vel.z > 0)
					{
						entryDistance.z = z - (pos.z + halfSize.z);
						exitDistance.z = (z + Block::SIZE) - (pos.z - halfSize.z);
					}
					else
					{
						entryDistance.z = (z + Block::SIZE) - (pos.z - halfSize.z);
						exitDistance.z = z - (pos.z + halfSize.z);
					}

					Vec3 entryTime;
					Vec3 exitTime;

					if (vel.x == 0.0f)
					{
						entryTime.x = -std::numeric_limits<float>::infinity();
						exitTime.x = std::numeric_limits<float>::infinity();
					}
					else
					{
						entryTime.x = entryDistance.x / vel.x;
						exitTime.x = exitDistance.x / vel.x;
					}

					if (vel.y == 0.0f)
					{
						entryTime.y = -std::numeric_limits<float>::infinity();
						exitTime.y = std::numeric_limits<float>::infinity();
					}
					else
					{
						entryTime.y = entryDistance.y / vel.y;
						exitTime.y = exitDistance.y / vel.y;
					}

					if (vel.z == 0.0f)
					{
						entryTime.z = -std::numeric_limits<float>::infinity();
						exitTime.z = std::numeric_limits<float>::infinity();
					}
					else
					{
						entryTime.z = entryDistance.z / vel.z;
						exitTime.z = exitDistance.z / vel.z;
					}

					// Finding max entry to find the most movement possible before collision on any axis.
					float entry = std::max(entryTime.x, std::max(entryTime.y, entryTime.z));
					float exit = std::min(exitTime.x, std::min(exitTime.y, exitTime.z));

					// No collision
					if ((entry > exit) 
						// Don't know if this should be there. I think it doesn't register a collision if the entity is already inside it.
						// I guess finding the normal doesn't work if it happens. But I also already handle that case.
						|| ((entryTime.x < 0.0f) && (entryTime.y < 0.0f) && (entryTime.z < 0.0f)) 
						|| (entryTime.x > 1.0f)
						|| (entryTime.y > 1.0f)
						|| (entryTime.z > 1.0f)
					// Closer collision already found.
						|| (entry > result.entryTime))
					{
						continue;
					}
					else
					{
						result.entryTime = entry;
						// This just finds the max entryTime again. Could optimize this.
						if (entryTime.x > entryTime.y && entryTime.x > entryTime.z)
						{
							if (entryDistance.x < 0.0f)
							{
								result.normal.x = 1.0f;
								result.normal.y = 0.0f;
								result.normal.z = 0.0f;
							}
							else
							{
								result.normal.x = -1.0f;
								result.normal.y = 0.0f;
								result.normal.z = 0.0f;
							}
						}
						else if (entryTime.y > entryTime.x && entryTime.y > entryTime.z)
						{
							if (entryDistance.y < 0.0f)
							{
								result.normal.x = 0.0f;
								result.normal.y = 1.0f;
								result.normal.z = 0.0f;
							}
							else
							{
								result.normal.x = 0.0f;
								result.normal.y = -1.0f;
								result.normal.z = 0.0f;
							}
						}
						else
						{
							if (entryDistance.z < 0.0f)
							{
								result.normal.x = 0.0f;
								result.normal.y = 0.0f;
								result.normal.z = 1.0f;
							}
							else
							{
								result.normal.x = 0.0f;
								result.normal.y = 0.0f;
								result.normal.z = -1.0f;
							}
						}
					}				
				}
			}
		}
	}

	return result;
}

bool PhysicsSystem::isBlockVsAabbCollision(const Vec3& blockPos, const Vec3& pos, const Vec3& halfSize)
{
	return (pos.x - halfSize.x < blockPos.x + Block::SIZE) && (pos.x + halfSize.x > blockPos.x)
		&& (pos.y - halfSize.y < blockPos.y + Block::SIZE) && (pos.y + halfSize.y > blockPos.y)
		&& (pos.z - halfSize.z < blockPos.z + Block::SIZE) && (pos.z + halfSize.z > blockPos.z);
}
