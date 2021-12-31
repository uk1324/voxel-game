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
#include <algorithm>

// TODO: Make an actuall AABB class that will have utility function like top bottom and maybe store the AABB as a min and max
// instead of center and size.

#include <imgui.h>

void PhysicsSystem::update(const Time& time, EntityManager& entityManager, const ChunkSystem& chunkSystem)
{
	for (auto& [entity, velocityComponent] : entityManager.getComponents<PhysicsVelocity>())
	{
		Vec3& entityVel = velocityComponent.value;
		Vec3& entityPos = entityManager.entityGetComponent<Position>(entity).value;
		bool& isEntityGrounded = entityManager.entityGetComponent<Grounded>(entity).value;
		const PhysicsAabbCollider& collider = entityManager.entityGetComponent<PhysicsAabbCollider>(entity);

		entityVel += Vec3::down * gravity * time.deltaTime();
		entityVel.y *= 0.98;

		if (isEntityGrounded)
		{
			entityVel.x *= 0.85;
			entityVel.z *= 0.85;
		}
		else
		{
			entityVel.x *= 0.98;
			entityVel.z *= 0.98;
		}

		isEntityGrounded = false;

		Vec3 movement = entityVel;
		for (int i = 0; i < 3; i++)
		{
			TerrainCollision collision = aabbVsTerrainCollision(chunkSystem, entityPos + collider.centerOffset, collider.size, entityVel);

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
				isEntityGrounded = true;
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
	}



	return;

	for (auto& [entity, velocityComponent] : entityManager.getComponents<PhysicsVelocity>())
	{
		Vec3& entityVel = velocityComponent.value;
		Vec3& entityPos = entityManager.entityGetComponent<Position>(entity).value;
		bool& isEntityGrounded = entityManager.entityGetComponent<Grounded>(entity).value;
		const PhysicsAabbCollider& collider = entityManager.entityGetComponent<PhysicsAabbCollider>(entity);

		ImGui::Begin("gravity");
		ImGui::SliderFloat("gravity", &gravity, 0, 20);
		ImGui::End();


		entityVel += Vec3::down * gravity * time.deltaTime();
		if (isEntityGrounded)
		{
			entityVel.x *= 0.85;
			entityVel.z *= 0.85;
		}
		else
		{
			entityVel.x *= 0.98;
			entityVel.z *= 0.98;
		}

		float entryAbc = 1.0f;
		Vec3 normal(0, 0, 0);

		Vec3 vel = entityVel;

		auto abc = [&]()
		{

			const Vec3 pos = entityPos + collider.centerOffset;
			const Vec3 halfSize = collider.size / 2.0f;

			const Vec3 newPos = pos + entityVel;

			Vec3 start;
			Vec3 end;

			if (pos.x < newPos.x)
			{
				start.x = pos.x;
				end.x = newPos.x;
			}
			else
			{
				start.x = newPos.x;
				end.x = pos.x;
			}

			if (pos.y < newPos.y)
			{
				start.y = pos.y;
				end.y = newPos.y;
			}
			else
			{
				start.y = newPos.y;
				end.y = pos.y;
			}

			if (pos.z < newPos.z)
			{
				start.z = pos.z;
				end.z = newPos.z;
			}
			else
			{
				start.z = newPos.z;
				end.z = pos.z;
			}

			start -= halfSize;
			end += halfSize;
			start.apply(floor);
			end.apply(ceil);

			for (float z = start.z; z < end.z; z += Block::SIZE)
			{
				for (float y = start.y; y < end.y; y += Block::SIZE)
				{
					for (float x = start.x; x < end.x; x += Block::SIZE)
					{
						auto isCollision = [](const ChunkSystem& a, const Vec3& pos, const Vec3& halfSize, float x, float y, float z)
						{
							const bool xCollision = (pos.x - halfSize.x < x + Block::SIZE) && (pos.x + halfSize.x > x);
							const bool yCollision = (pos.y - halfSize.y < y + Block::SIZE) && (pos.y + halfSize.y > y);
							const bool zCollision = (pos.z - halfSize.z < z + Block::SIZE) && (pos.z + halfSize.z > z);
							return a.get(x, y, z) != BlockType::Air && xCollision && yCollision && zCollision;
						};

						if (chunkSystem.get(x, y, z).isSolid() && isCollision(chunkSystem, pos, halfSize, x, y, z) == false)
						{
							/*Debug::drawCube(Vec3(x, y, z) + Vec3(Block::SIZE / 2.0f), Vec3(1, 1, 1));*/

							Vec3 entryDistance;
							Vec3 exitDistance;

							if (vel.x > 0)
							{
								entryDistance.x = (x - (pos.x + halfSize.x));
								exitDistance.x = (x + Block::SIZE) - (pos.x - halfSize.x);
							}
							else
							{
								entryDistance.x = (x + Block::SIZE) - (pos.x - halfSize.x);
								exitDistance.x = (x - (pos.x + halfSize.x));
							}

							if (vel.y > 0)
							{
								entryDistance.y = (y - (pos.y + halfSize.y));
								exitDistance.y = (y + Block::SIZE) - (pos.y - halfSize.y);
							}
							else
							{
								entryDistance.y = (y + Block::SIZE) - (pos.y - halfSize.y);
								exitDistance.y = (y - (pos.y + halfSize.y));
							}

							if (vel.z > 0)
							{
								entryDistance.z = (z - (pos.z + halfSize.z));
								exitDistance.z = (z + Block::SIZE) - (pos.z - halfSize.z);
							}
							else
							{
								entryDistance.z = (z + Block::SIZE) - (pos.z - halfSize.z);
								exitDistance.z = (z - (pos.z + halfSize.z));
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

							float entry = std::max(entryTime.x, std::max(entryTime.y, entryTime.z));
							float exit = std::min(exitTime.x, std::max(exitTime.y, exitTime.z));

							Vec3 n;

							if (entry > exit || entryTime.x < 0.0f && entryTime.y < 0.0f && entryTime.z < 0.0f || entryTime.x > 1.0f || entryTime.y > 1.0f || entryTime.z > 1.0f)
							{
								n.x = 0.0f;
								n.y = 0.0f;
								n.z = 0.0f;
								if (entry < entryAbc)
								{
									entryAbc = 1.0;
									normal = n;
								}
								continue;
							}
							else
							{
								//ASSERT_NOT_REACHED();
								if (entryTime.x > entryTime.y && entryTime.x > entryTime.z)
								{
									if (entryDistance.x < 0.0f)
									{
										n.x = 1.0f;
										n.y = 0.0f;
										n.z = 0.0f;
									}
									else
									{
										n.x = -1.0f;
										n.y = 0.0f;
										n.z = 0.0f;
									}
								}
								else if (entryTime.y > entryTime.x && entryTime.y > entryTime.z)
								{
									if (entryDistance.y < 0.0f)
									{
										n.x = 0.0f;
										n.y = 1.0f;
										n.z = 0.0f;
									}
									else
									{
										n.x = 0.0f;
										n.y = -1.0f;
										n.z = 0.0f;
									}
								}
								else
								{
									if (entryDistance.z < 0.0f)
									{
										n.x = 0.0f;
										n.y = 0.0f;
										n.z = 1.0f;
									}
									else
									{
										n.x = 0.0f;
										n.y = 0.0f;
										n.z = -1.0f;
									}
								}
							}

							if (entry < entryAbc)
							{
								normal = n;
								entryAbc = entry;
							}
						}

					}
				}
			}

		};

		const Vec3 halfSize = collider.size / 2.0f;
		Vec3 start = entityPos + collider.centerOffset - halfSize;
		Vec3 end = entityPos + collider.centerOffset + halfSize;
		start.apply(floor);
		end.apply(ceil);
		start.y -= 1.0f;
		//end.y = start.y;
		float friction = 1.0f;

		for (float x = start.x; x < end.x; x++)
		{
			for (float z = start.z; z < end.z; z++)
			{
				Debug::drawCube(Vec3(x, start.y, z) + Vec3(Block::SIZE / 2.0f), Vec3(1, 1, 1));

				if (chunkSystem.get(x, start.y, z) == BlockType::Cobblestone)
				{
					std::cout << "yes";
					friction = 0.95;
				}
			}
		}

		//if (friction == 1.0f)
		//if (true)
		//{
		//	/*entityVel.x *= 0.85;
		//	entityVel.z *= 0.85;*/
		//	entityVel.x *= 0.85;
		//	entityVel.z *= 0.85;
		//}
		//else
		//{
		//	entityVel.x *= friction;
		//	entityVel.z *= friction;
		//}
		//entityVel.y *= 0.98;

		bool xResolved = false, yResolved = false, zResolved = false;

		bool& isGrounded = entityManager.entityGetComponent<Grounded>(entity).value;
		isGrounded = false;

		//float remainingTime = 1.0f;

		normal = Vec3(0);

		//for (int i = 0; i < 3; i++)
		//{

		//	entityVel.x = entityVel.x * (1 - abs(normal.x)) * remainingTime;
		//	entityVel.y = entityVel.y * (1 - abs(normal.y)) * remainingTime;
		//	entityVel.z = entityVel.z * (1 - abs(normal.z)) * remainingTime;

		//	entryAbc = 1.0f;
		//	normal = Vec3(0);
		//	abc();

		//	ASSERT(entryAbc >= 0.0);

		//	if (normal.y == 1)
		//	{
		//		isGrounded = true;
		//	}

		//	if (normal.x != 0.0f)
		//	{
		//		if (xResolved)
		//			ASSERT_NOT_REACHED();
		//		xResolved = true;
		//	}
		//	else if (normal.y != 0.0f)
		//	{
		//		if (yResolved)
		//			ASSERT_NOT_REACHED();
		//		yResolved = true;
		//	}
		//	else if (normal.z != 0.0f)
		//	{
		//		if (zResolved)
		//			ASSERT_NOT_REACHED();
		//		zResolved = true;
		//	}

		//	entityPos += entityVel * (entryAbc);

		//	if (entryAbc < 1.0f)
		//	{
		//		entityPos += normal * 0.001;
		//	}

		//	remainingTime = 1.0f - entryAbc;

		//	if (remainingTime <= 0)
		//		break;
		//}

		//if (xResolved || yResolved || zResolved)
		//	std::cout << xResolved << yResolved << zResolved << '\n';

		for (int i = 0; i < 3; i++)
		{
			entryAbc = 1.0f;
			normal = Vec3(0);
			abc();
			//if (entryAbc != 1.0f)
			{
				float remainingtime = 1.0f - entryAbc;
				std::cout << "entryAbc: " << entryAbc << ' ' << remainingtime << '\n';
				entityPos += vel * (entryAbc - 0.01);

				if (normal.y == 1)
				{
					isGrounded = true;
				}

				if (normal.x != 0.0f)
				{
					if (xResolved)
						std::cout << "ABC";
					entityVel.x = 0.0f;
					vel.x = 0.0f;
					xResolved = true;
				}
				else if (normal.y != 0.0f)
				{
					if (yResolved)
						std::cout << "ABC";
					entityVel.y = 0.0f;
					vel.y = 0.0f;
					yResolved = true;
				}
				else if (normal.z != 0.0f)
				{
					if (zResolved)
						std::cout << "ABC";
					entityVel.z = 0.0f;
					vel.z = 0.0f;
					zResolved = true;
				}
				vel *= remainingtime;

				if (remainingtime <= 0.0f)
				{
					break;
				}
			}
		}
		//if (!xResolved && !yResolved && !zResolved)
		//	entityPos += entityVel;

		Debug::drawCube(entityPos + collider.centerOffset, collider.size, Vec3(0, 1, 0));
	}


}

PhysicsSystem::TerrainCollision PhysicsSystem::aabbVsTerrainCollision(const ChunkSystem& chunkSystem, const Vec3& pos, const Vec3& size, Vec3 vel)
{
	TerrainCollision result;
	result.entryTime = 1.0f;
	result.normal = Vec3(0.0f);

	Vec3 newPos = pos + vel;
	const Vec3 halfSize = size / 2.0f;

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
				Debug::drawCube(Vec3(x, y, z) + Vec3(Block::SIZE / 2.0f), Vec3(1, 1, 1));

				if (chunkSystem.get(x, y, z).isSolid()
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
