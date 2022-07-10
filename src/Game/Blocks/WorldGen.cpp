#include <Game/Blocks/WorldGen.hpp>
#include <Math/Other.hpp>
#include <Utils/Assertions.hpp>
#include <imgui.h>
#include <optional>

#include <random>

WorldGen::WorldGen(unsigned int seed)
	: m_noise(0)
	, m_tree(Vec3I(5, 7, 5))
	, m_flower(Vec3I(1, 1, 1))
{
	m_tree.set(Vec3I(2, 0, 2), BlockType::Log);
	m_tree.set(Vec3I(2, 1, 2), BlockType::Log);
	m_tree.set(Vec3I(2, 2, 2), BlockType::Log);
	m_tree.set(Vec3I(2, 3, 2), BlockType::Log);
	m_tree.set(Vec3I(2, 4, 2), BlockType::Log);

	m_tree.set(Vec3I(1, 3, 0), BlockType::Leaves);
	m_tree.set(Vec3I(2, 3, 0), BlockType::Leaves);
	m_tree.set(Vec3I(3, 3, 0), BlockType::Leaves);

	m_tree.set(Vec3I(0, 3, 1), BlockType::Leaves);
	m_tree.set(Vec3I(1, 3, 1), BlockType::Leaves);
	m_tree.set(Vec3I(2, 3, 1), BlockType::Leaves);
	m_tree.set(Vec3I(3, 3, 1), BlockType::Leaves);
	m_tree.set(Vec3I(4, 3, 1), BlockType::Leaves);

	m_tree.set(Vec3I(0, 3, 2), BlockType::Leaves);
	m_tree.set(Vec3I(1, 3, 2), BlockType::Leaves);
	m_tree.set(Vec3I(3, 3, 2), BlockType::Leaves);
	m_tree.set(Vec3I(4, 3, 2), BlockType::Leaves);

	m_tree.set(Vec3I(0, 3, 3), BlockType::Leaves);
	m_tree.set(Vec3I(1, 3, 3), BlockType::Leaves);
	m_tree.set(Vec3I(2, 3, 3), BlockType::Leaves);
	m_tree.set(Vec3I(3, 3, 3), BlockType::Leaves);
	m_tree.set(Vec3I(4, 3, 3), BlockType::Leaves);

	m_tree.set(Vec3I(1, 3, 4), BlockType::Leaves);
	m_tree.set(Vec3I(2, 3, 4), BlockType::Leaves);
	m_tree.set(Vec3I(3, 3, 4), BlockType::Leaves); 

	m_tree.set(Vec3I(1, 4, 0), BlockType::Leaves);
	m_tree.set(Vec3I(2, 4, 0), BlockType::Leaves);
	m_tree.set(Vec3I(3, 4, 0), BlockType::Leaves); 

	m_tree.set(Vec3I(0, 4, 1), BlockType::Leaves);
	m_tree.set(Vec3I(1, 4, 1), BlockType::Leaves);
	m_tree.set(Vec3I(2, 4, 1), BlockType::Leaves);
	m_tree.set(Vec3I(3, 4, 1), BlockType::Leaves);
	m_tree.set(Vec3I(4, 4, 1), BlockType::Leaves); 

	m_tree.set(Vec3I(0, 4, 2), BlockType::Leaves);
	m_tree.set(Vec3I(1, 4, 2), BlockType::Leaves);
	m_tree.set(Vec3I(3, 4, 2), BlockType::Leaves);
	m_tree.set(Vec3I(4, 4, 2), BlockType::Leaves); 

	m_tree.set(Vec3I(0, 4, 3), BlockType::Leaves);
	m_tree.set(Vec3I(1, 4, 3), BlockType::Leaves);
	m_tree.set(Vec3I(2, 4, 3), BlockType::Leaves);
	m_tree.set(Vec3I(3, 4, 3), BlockType::Leaves);
	m_tree.set(Vec3I(4, 4, 3), BlockType::Leaves);

	m_tree.set(Vec3I(1, 4, 4), BlockType::Leaves);
	m_tree.set(Vec3I(2, 4, 4), BlockType::Leaves);
	m_tree.set(Vec3I(3, 4, 4), BlockType::Leaves);

	m_tree.set(Vec3I(2, 5, 2), BlockType::Leaves);
	m_tree.set(Vec3I(2, 5, 2), BlockType::Leaves);
	m_tree.set(Vec3I(2, 5, 1), BlockType::Leaves);
	m_tree.set(Vec3I(2, 5, 3), BlockType::Leaves);
	m_tree.set(Vec3I(1, 5, 2), BlockType::Leaves);
	m_tree.set(Vec3I(3, 5, 2), BlockType::Leaves);
	m_tree.set(Vec3I(3, 5, 3), BlockType::Leaves); 

	m_tree.set(Vec3I(2, 6, 2), BlockType::Leaves);
	m_tree.set(Vec3I(2, 6, 2), BlockType::Leaves);
	m_tree.set(Vec3I(2, 6, 1), BlockType::Leaves);
	m_tree.set(Vec3I(2, 6, 3), BlockType::Leaves);
	m_tree.set(Vec3I(1, 6, 2), BlockType::Leaves);
	m_tree.set(Vec3I(3, 6, 2), BlockType::Leaves);

	m_flower.set(Vec3I(0, 0, 0), BlockType::Flower);
}

float cave1 = 0.1;
float cave2 = 0.2;
float cave3 = 0.0;

void WorldGen::updateDebugConfig()
{
	return;
	ImGui::Begin("Cave generation");

	ImGui::SliderFloat("scale", &cave1, 0.0f, 1.0f);
	ImGui::SliderFloat("epsilon", &cave2, 0.0f, 1.0f);
	ImGui::SliderFloat("offset", &cave3, 0.0f, 1.0f);


	ImGui::End();

	return;
	ImGui::Begin("World generation");

	ImGui::End();

	return;
	ImGui::Begin("World generation");

	ImGui::SliderFloat("terrainAmplitude", &terrainAmplitude, 1.0f, 50.0f);

	ImGui::Text("Height map");
	ImGui::SliderFloat("heightMapScale", &heightMapScale, 0.005f, 0.1f);
	ImGui::SliderInt("heightMapOctaves", &heightMapOctaves, 0, 6);
	ImGui::SliderFloat("heightMapOctaveLacunarity", &heightMapOctaveLacunarity, 0.05f, 2.0f);
	ImGui::SliderFloat("heightMapOctavePersitence", &heightMapOctavePersitence, 0.05f, 0.5f);

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	ImGui::Text("Noise");
	ImGui::SliderFloat("noiseScale", &overhangNoiseScale, 0.005f, 0.1f);
	ImGui::SliderInt("noiseOctaves", &overhangNoiseOctaves, 0, 6);
	ImGui::SliderFloat("noiseOctaveLacunarity", &overhangNoiseOctaveLacunarity, 0.05f, 2.0f);
	ImGui::SliderFloat("noiseOctavePersitence", &overhangNoiseOctavePersitence, 0.05f, 0.5f);

	ImGui::End();
}

// Maybe later optimize the iteration access of Chunks so I can generate a singe noise value for each y.
// Iteration order x z y or z x y
void WorldGen::generateChunk(Chunk& chunk, const Vec3I& chunkPos) const
{
	auto structures = getStructures(chunkPos);

	structures.erase(
		std::remove_if(structures.begin(), structures.end(), [this, chunkPos](const Structure& structure) {
			return (structure.pos.x + structure.data->size.x < chunkPos.x * Chunk::SIZE_X)
				|| (structure.pos.x > chunkPos.x * Chunk::SIZE_X + Chunk::SIZE_X)
				|| (structure.pos.y + structure.data->size.y < chunkPos.y * Chunk::SIZE_Y)
				|| (structure.pos.y > chunkPos.y * Chunk::SIZE_Y + Chunk::SIZE_Y)
				|| (structure.pos.z + structure.data->size.z < chunkPos.z * Chunk::SIZE_Z)
				|| (structure.pos.z > chunkPos.z * Chunk::SIZE_Z + Chunk::SIZE_Z);
		}),
		structures.end()
	);

	for (int32_t z = 0; z < Chunk::SIZE_Z; z++)
	{
		for (int32_t x = 0; x < Chunk::SIZE_X; x++)
		{
			const Vec2 posXz(x + chunkPos.x * Chunk::SIZE_X, z + chunkPos.z * Chunk::SIZE_Z);
			float heightMapNoiseValue = heightMapNoise(posXz);

			const Vec3 pos(Vec3(x, 0, z) + Vec3(chunkPos) * Chunk::SIZE_V - Vec3(0, heightOffset, 0));
			float nextTerrainNoise = floor(terrainNoise(heightMapNoiseValue, pos));
			for (int32_t y = 0; y < Chunk::SIZE_Y; y++)
			{
				const Vec3I posInChunk(x, y, z);
				const Vec3 pos(Vec3(posInChunk) + Vec3(chunkPos) * Chunk::SIZE_V - Vec3(0, heightOffset, 0));

				//if (pos.y == -5)
				//{
				//	chunk.set(posInChunk, BlockType::Grass);
				//}
				//else
				//{
				//	chunk.set(posInChunk, BlockType::Air);
				//}

				//continue;

				//if (abs(m_noise.value3d(pos * cave1) - cave3) < cave2)
				//{
				//	chunk.set(posInChunk, BlockType::Air);
				//}
				//else
				//{
				//	chunk.set(posInChunk, BlockType::Stone);
				//}

				//continue;
				auto n = floor(terrainNoise(heightMapNoiseValue, pos + Vec3::up));
				if (pos.y <= nextTerrainNoise)
				{
					if (pos.y >= n)
					{
						chunk.set(posInChunk, BlockType::Grass);
					}
					else
					{
						if (pos.y + stoneDepth <= floor(terrainNoise(heightMapNoiseValue, pos + Vec3::up * stoneDepth)))
						{
							chunk.set(posInChunk, BlockType::Stone);
						}
						else
						{
							chunk.set(posInChunk, BlockType::Dirt);
						}
					}
				}
				else
				{
					chunk.set(posInChunk, BlockType::Air);
				}
				nextTerrainNoise = n;
			}
		}
	}

	for (const auto& structure : structures)
	{
		const auto posInChunk = structure.pos - chunkPos * Chunk::SIZE_INT;
		const auto minX = std::clamp(posInChunk.x, 0, Chunk::SIZE_X - 1);
		const auto maxX = std::clamp(posInChunk.x + structure.data->size.x, 0, Chunk::SIZE_X);
		const auto minY = std::clamp(posInChunk.y, 0, Chunk::SIZE_Y - 1);
		const auto maxY = std::clamp(posInChunk.y + structure.data->size.y, 0, Chunk::SIZE_Y);
		const auto minZ = std::clamp(posInChunk.z, 0, Chunk::SIZE_Z - 1);
		const auto maxZ = std::clamp(posInChunk.z + structure.data->size.z, 0, Chunk::SIZE_Z);

		for (size_t z = minZ; z < maxZ; z++)
		{
			for (size_t y = minY; y < maxY; y++)
			{
				for (size_t x = minX; x < maxX; x++)
				{
					const Vec3I blockPosInChunk(x, y, z);
					const auto block = structure.data->get(blockPosInChunk - posInChunk);
					if (block != BlockType::Air)
					{
						chunk.set(blockPosInChunk, block);
					}
				}
			}
		}
	}
}

float WorldGen::overhangNoise(const Vec3& pos) const
{
	return m_noise.accumulatedValue3d(pos * overhangNoiseScale, overhangNoiseOctaves, overhangNoiseOctaveLacunarity, overhangNoiseOctavePersitence);
}

float WorldGen::heightMapNoise(const Vec2& posXz) const
{
	return m_noise.accumulatedValue2d(posXz * heightMapScale, heightMapOctaves, heightMapOctaveLacunarity, heightMapOctavePersitence);
}

float WorldGen::terrainNoise(float heightMapNoiseValue, const Vec3& pos) const
{
	return (heightMapNoiseValue + overhangNoise(pos)) * terrainAmplitude;
}

// This probably is a pretty bad hash function but it doesn't look like it generates any readily apparent patterns.
float WorldGen::hash(const Vec2I& vec) const
{
	// Don't use mt19937_64 it is really slow.
	// ranlux24 is pretty fast the chunk generation time is only 0.6 of a millisecond slower than just returing 0.5 which generated.
	// no structres.
	std::ranlux24 gen(vec.x * 1123 + vec.y);
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
	return distribution(gen);
}

std::vector<Structure> WorldGen::getStructures(const Vec3I& chunkPos) const
{
	std::vector<Structure> structures;

	Vec2I start = (Vec2I(chunkPos.x, chunkPos.z) - Vec2I(1)) * Vec2I(Chunk::SIZE_X, Chunk::SIZE_Z);
	Vec2I end = (Vec2I(chunkPos.x, chunkPos.z) + Vec2I(1)) * Vec2I(Chunk::SIZE_X, Chunk::SIZE_Z);

	for (int32_t x = start.x; x < end.x; x++)
	{
		for (int32_t z = start.y; z < end.y; z++)
		{
			if (hash(Vec2I(x, z)) < treeSpawnChance)
			{
				int32_t treeY = findGroundY(Vec2(x + m_tree.size.x / 2, z + m_tree.size.z / 2));
				structures.push_back(Structure(Vec3I(x, floor(treeY), z), m_tree));
			}
			else if (hash(Vec2I(x, z)) > 0.96)
			{
				int32_t treeY = findGroundY(Vec2(x, z));
				structures.push_back(Structure(Vec3I(x, floor(treeY), z), m_flower));
			}
		}
	}

	return structures;
}


// Takes around 8 iterations to find root.
// A cool thing about bisection is that it only finds the roots on one size so trees can't spawn on the bottom of things.
int32_t WorldGen::findGroundY(const Vec2& posXz) const
{
	float heightValue = heightMapNoise(posXz);

	int min = 0;
	int max = Chunk::SIZE_Y;

	auto at = [heightValue, posXz, this](float y) -> float
	{
		y -= heightOffset;
		const float height = terrainNoise(heightValue, Vec3(posXz.x, y, posXz.y));
		return height - y;
	};

	int mid = (min + max) / 2;
	while (max - min > 1)
	{
		float atM = at(mid);
		float atA = at(min);

		float signA = signNonZero(atA);
		float signM = signNonZero(atM);

		if (signA == signM)
		{ 
			min = mid;
		}
		else
		{
			max = mid;
		}

		mid = (min + max) / 2;
	}

	return mid + 1;
}

// The actual min and max height values are differnt because the perlin noise isn't normalized.
// The values should be equal to PerlinNoise::accumulatedValueMax but they are most of the time the half of this
// because the perlin noise range is -1 to 1 the values cancel out.
// The current terrain looks good so I won't change it for now.
float WorldGen::maxHeight() const
{
	return terrainAmplitude;
}

float WorldGen::minHeight() const
{
	return -terrainAmplitude;
}
