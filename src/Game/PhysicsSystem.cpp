#include <Game/PhysicsSystem.hpp>
#include <Game/Components/Position.hpp>


#include <Game/Rendering/RenderingSystem.hpp>

PhysicsSystem::PhysicsSystem(Scene& scene)
{
	scene.entityManager.registerComponent<PhysicsVelocity>();
	scene.entityManager.registerComponent<PhysicsAabbCollider>();
}

#include <iostream>

void PhysicsSystem::update(EntityManager& entityManger, const ChunkSystem& chunkSystem)
{
	for (auto& [entity, velocityComponent] : entityManger.getComponents<PhysicsVelocity>())
	{
		Vec3& vel = velocityComponent.value;
		Vec3& pos = entityManger.entityGetComponent<Position>(entity).value;

		vel = Vec3(0, -0.05, 0);
		pos += vel;
	}

	for (const auto& [entity, collider] : entityManger.getComponents<PhysicsAabbCollider>())
	{
		Vec3& entityPos = entityManger.entityGetComponent<Position>(entity).value;
		Vec3& entityVel = entityManger.entityGetComponent<PhysicsVelocity>(entity).value;
		const Vec3 pos = entityManger.entityGetComponent<Position>(entity).value + collider.centerOffset;
		//const Vec3 vel = entityManger.entityGetComponent<Veloc>(entity).value + collider.centerOffset;

		const Vec3 halfSize = collider.size / 2.0f;

		const Vec3I voxelPos = pos;
		const Vec3I start = voxelPos - Vec3(5, 5, 5);
		const Vec3I end = voxelPos + Vec3(5, 5, 5);

		for (float z = start.z; z < end.z; z += Block::SIZE)
		{
			for (float y = start.y; y < end.y; y += Block::SIZE)
			{
				for (float x = start.x; x < end.x; x += Block::SIZE)
				{
					//std::cout << Vec3(x, y, z) << '\n';
					if (chunkSystem.get(x, y, z) != BlockType::Air)
					{
						//std::cout << Vec3(x, y, z) << '\n';

						Debug::drawCube(Vec3(x, y, z) + Vec3(Block::SIZE) / 2);
						

						auto isCollision = [](const ChunkSystem& a, const Vec3& pos, const Vec3& halfSize, float x, float y, float z)
						{
							const bool xCollision = (pos.x - halfSize.x < x + Block::SIZE) && (pos.x + halfSize.x > x);
							const bool yCollision = (pos.y - halfSize.y < y + Block::SIZE) && (pos.y + halfSize.y > y);
							const bool zCollision = (pos.z - halfSize.z < z + Block::SIZE) && (pos.z + halfSize.z > z);
							return a.get(x, y, z) != BlockType::Air && xCollision && yCollision && zCollision;
						};

						const bool xCollision = (pos.x - halfSize.x < x + Block::SIZE) && (pos.x + halfSize.x > x);
						const bool yCollision = (pos.y - halfSize.y < y + Block::SIZE) && (pos.y + halfSize.y > y);
						const bool zCollision = (pos.z - halfSize.z < z + Block::SIZE) && (pos.z + halfSize.z > z);

						const float yPosDistance = abs((pos.y - halfSize.y) - (y + Block::SIZE));
						const float yNegDistance = abs((pos.y + halfSize.y) - y);

						const Vec3 posDistance = Vec3((pos - halfSize) - (Vec3(x, y, z) + Vec3(Block::SIZE))).applied(abs);
						const Vec3 negDistance = Vec3((pos + halfSize) - Vec3(x, y, z)).applied(abs); 

						//Debug::drawPoint(entityPos);

						const std::vector<float> distance = {
							posDistance.x,
							posDistance.y,	
							posDistance.z,
							negDistance.x,
							negDistance.y,
							negDistance.z
						};

						size_t minIndex = 126126126120;
						float min = std::numeric_limits<float>::infinity();

						enum class Distance
						{
							PosX,
							PosY,
							PosZ,
							NegX,
							NegY,
							NegZ,
						};

						const std::vector<bool (*)(const ChunkSystem&, float, float, float)> a = {
							[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x + 1, y, z) == BlockType::Air; },
							[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x, y + 1, z) == BlockType::Air; },
							[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x, y, z + 1) == BlockType::Air; },
							[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x - 1, y, z) == BlockType::Air; },
							[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x, y - 1, z) == BlockType::Air; },
							[](const ChunkSystem& c, float x, float y, float z) -> bool { return c.get(x, y, z - 1) == BlockType::Air; },
						};

						std::vector<size_t> minIndices;

						for (size_t i = 0; i < distance.size(); i++)
						{
							bool yes = true;

							switch (static_cast<Distance>(i))
							{
								case Distance::PosX:
									for (float y = start.y; y < end.y; y++)
									{
										if (isCollision(chunkSystem, pos, halfSize - Vec3(0.05), x + 1, y, z))
										{
											yes = false;
										}
									}

									break;
								case Distance::PosY:
									break;
								case Distance::PosZ:
									for (float y = start.y; y < end.y; y++)
									{
										if (isCollision(chunkSystem, pos, halfSize - Vec3(0.05), x, y, z + 1))
										{
											Debug::drawCube(Vec3(x, y, z + 1) + Vec3(0.5), Vec3(1), Vec3(0, 0, 1));
											yes = false;
										}
									}
									break;
								case Distance::NegX:
									for (float y = start.y; y < end.y; y++)
									{
										if (isCollision(chunkSystem, pos, halfSize - Vec3(0.05), x - 1, y, z))
										{
											yes = false;
										}
									}
									break;
								case Distance::NegY:
									break;
								case Distance::NegZ:
									for (float y = start.y; y < end.y; y++)
									{
										if (isCollision(chunkSystem, pos, halfSize - Vec3(0.05), x, y, z - 1))
										{
											yes = false;
										}
									}
									break;
							}
							//yes = true;
							if (distance[i] < min && a[i](chunkSystem, x, y, z) && yes)
							{
								minIndex = i;
								min = distance[i];
							}
						}

						if (minIndex == 126126126120)
						{
							continue;
						}

						if (min > 0.3)
						{
							continue;
						}

						auto isAlmostEqual = [](float x, float y)
						{
							return (std::abs(x - y) < 0.01 * std::max(std::abs(x), std::abs(y)));
						};

						std::vector<size_t> distIndices;

						for (size_t i = 0; i < distance.size(); i++)
						{
							if (isAlmostEqual(min, distance[i]))
							{
								distIndices.push_back(i);
							}
						}


						if (xCollision && yCollision && zCollision)
						{
							for (size_t d : distIndices)
							{
								switch (static_cast<Distance>(d))
								{
								case Distance::PosX:
									//if (chunkSystem.get(x + 1, y, z) != BlockType::Air)
								{
									entityPos.x = x + Block::SIZE + halfSize.x - collider.centerOffset.x;
									entityVel.x = 0;
								}
								break;

								case Distance::PosY:
									//if (chunkSystem.get(x, y + 1, z) != BlockType::Air)
								{
									entityPos.y = y + Block::SIZE + halfSize.y - collider.centerOffset.y;
									entityVel.y = 0;
								}
								break;

								case Distance::PosZ:
									//if (chunkSystem.get(x, y, z + 1) != BlockType::Air)
								{
									entityPos.z = z + Block::SIZE + halfSize.z - collider.centerOffset.z;
									entityVel.z = 0;
								}
								break;

								case Distance::NegX:
									//if (chunkSystem.get(x - 1, y, z) != BlockType::Air)
								{
									entityPos.x = x - halfSize.x - collider.centerOffset.x;
									entityVel.x = 0;
								}
								break;

								case Distance::NegY:
									//if (chunkSystem.get(x, y - 1, z) != BlockType::Air)
								{
									entityPos.y = y - halfSize.y - collider.centerOffset.y;
									entityVel.y = 0;
								}
								break;

								case Distance::NegZ:
									//if (chunkSystem.get(x, y, z - 1) != BlockType::Air)
								{
									entityPos.z = z - halfSize.z - collider.centerOffset.z;
									entityVel.z = 0;
								}
								break;
								}
							}
						}
					}
				}
			}
		}
		Debug::drawCube(entityPos + collider.centerOffset, collider.size, Vec3(0, 1, 0));
	}
}
