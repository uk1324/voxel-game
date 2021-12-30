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

	ImGui::Begin("gravity");
	ImGui::SliderFloat("gravity", &gravity, 0, 100);
	ImGui::End();

	for (auto& [entity, velocityComponent] : entityManager.getComponents<PhysicsVelocity>())
	{
		Vec3& entityVel = velocityComponent.value;
		Vec3& entityPos = entityManager.entityGetComponent<Position>(entity).value;
		bool& isEntityGrounded = entityManager.entityGetComponent<Grounded>(entity).value;
		const PhysicsAabbCollider& collider = entityManager.entityGetComponent<PhysicsAabbCollider>(entity);

		entityVel += Vec3::down * gravity * time.deltaTime();

		float entryAbc = 1.0f;
		Vec3 normal(0, 0, 0);

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
						if (chunkSystem.get(x, y, z).isSolid())
						{
							/*Debug::drawCube(Vec3(x, y, z) + Vec3(Block::SIZE / 2.0f), Vec3(1, 1, 1));*/

							Vec3 entryDistance;
							Vec3 exitDistance;

							if (entityVel.x > 0)
							{
								entryDistance.x = (x - (pos.x + halfSize.x));
								exitDistance.x = (x + Block::SIZE) - (pos.x - halfSize.x);
							}
							else
							{
								entryDistance.x = (x + Block::SIZE) - (pos.x - halfSize.x);
								exitDistance.x = (x - (pos.x + halfSize.x));
							}

							if (entityVel.y > 0)
							{
								entryDistance.y = (y - (pos.y + halfSize.y));
								exitDistance.y = (y + Block::SIZE) - (pos.y - halfSize.y);
							}
							else
							{
								entryDistance.y = (y + Block::SIZE) - (pos.y - halfSize.y);
								exitDistance.y = (y - (pos.y + halfSize.y));
							}

							if (entityVel.z > 0)
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

							if (entityVel.x == 0.0f)
							{
								entryTime.x = -std::numeric_limits<float>::infinity();
								exitTime.x = std::numeric_limits<float>::infinity();
							}
							else
							{
								entryTime.x = entryDistance.x / entityVel.x;
								exitTime.x = exitDistance.x / entityVel.x;
							}

							if (entityVel.y == 0.0f)
							{
								entryTime.y = -std::numeric_limits<float>::infinity();
								exitTime.y = std::numeric_limits<float>::infinity();
							}
							else
							{
								entryTime.y = entryDistance.y / entityVel.y;
								exitTime.y = exitDistance.y / entityVel.y;
							}

							if (entityVel.z == 0.0f)
							{
								entryTime.z = -std::numeric_limits<float>::infinity();
								exitTime.z = std::numeric_limits<float>::infinity();
							}
							else
							{
								entryTime.z = entryDistance.z / entityVel.z;
								exitTime.z = exitDistance.z / entityVel.z;
							}

							std::vector<float> d = { entryTime.x, entryTime.y, entryTime.z };
							std::vector<float> e = { exitTime.x, exitTime.y, exitTime.z };

							float entry = *std::max_element(d.begin(), d.end());
							float exit = *std::min_element(e.begin(), e.end());;

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

		bool xResolved = false, yResolved = false, zResolved = false;

		bool& isGrounded = entityManager.entityGetComponent<Grounded>(entity).value;
		isGrounded = false;

		for (int i = 0; i < 3; i++)
		{
			entryAbc = 1.0f;
			normal = Vec3(0);
			abc();
			if (entryAbc != 1.0f)
			{
				float remainingtime = (1.0f - entryAbc);
				entityPos += entityVel * (entryAbc - 0.01);

				if (normal.y == 1)
				{
					isGrounded = true;
				}

				if (normal.x != 0.0f)
				{
					if (xResolved)
						std::cout << "ABC";
					entityVel.x = 0.0f;
					xResolved = true;
				}
				else if (normal.y != 0.0f)
				{
					if (yResolved)
						std::cout << "ABC";
					entityVel.y = 0.0f;
					yResolved = true;
				}
				else if (normal.z != 0.0f)
				{
					if (zResolved)
						std::cout << "ABC";
					entityVel.z = 0.0f;
					zResolved = true;
				}
				entityVel = entityVel * remainingtime;
			}
		}
		if (xResolved || yResolved || zResolved)
			std::cout << xResolved << yResolved << zResolved << '\n';

		entityPos += entityVel;


		if (friction == 1.0f)
		{
			entityVel.x *= 0.85;
			entityVel.z *= 0.85;
		}
		else
		{
			entityVel.x *= friction;
			entityVel.z *= friction;
		}
		//entityVel.y *= 0.85;

		//Debug::drawCube(entityPos + collider.centerOffset, collider.size, Vec3(0, 1, 0));
	}

	/*for (auto& [entity, velocityComponent] : entityManager.getComponents<PhysicsVelocity>())
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

		entityVel += Vec3(0, -gravity, 0) * time.deltaTime();
		entityVel.x *= 0.85;
		entityVel.z *= 0.85;


		const Vec3 start = (colliderPos - halfSize).applied(floor);
		const Vec3 end = (colliderPos + halfSize).applied(ceil);

		for (float z = start.z; z < end.z; z += Block::SIZE)
		{
			for (float y = start.y; y < end.y; y += Block::SIZE)
			{
				for (float x = start.x; x < end.x; x += Block::SIZE)
				{
					const Vec3 voxelPos(x, y, z);
					//Debug::drawCube(voxelPos + Vec3(Block::SIZE / 2.0f), Vec3(1, 1, 1));

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

	}*/
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
