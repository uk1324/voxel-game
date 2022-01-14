#include "WorldGen.hpp"
#include "WorldGen.hpp"
#include "WorldGen.hpp"
#include "WorldGen.hpp"
#include <Game/Blocks/WorldGen.hpp>

#include <random>

#include <iostream>

WorldGen::WorldGen(unsigned int seed)
	: m_randomGenerator(seed)
	, m_randomDistribution(0.0f, 1.0f)
{
	for (auto& value : m_values)
	{
		value = randomFloat();
	}
}

void WorldGen::generateChunk(Chunk& chunk, const Vec3I& chunkPos)
{
	for (int32_t z = 0; z < Chunk::SIZE; z++)
	{
		for (int32_t y = 0; y < Chunk::SIZE; y++)
		{
			for (int32_t x = 0; x < Chunk::SIZE; x++)
			{
				Vec3I posInChunk(x, y, z);
				Vec3I pos = chunkPos * Chunk::SIZE + posInChunk;
				
				auto height = static_cast<int32_t>(
					noiseAt(static_cast<float>(x) / 5.0f) * 20
				);

				if (pos.y < height)
					chunk.set(posInChunk, BlockType::Cobblestone);
				else
					chunk.set(posInChunk, BlockType::Air);
			}
		}
	}
}

float WorldGen::randomFloat()
{
	return m_randomDistribution(m_randomGenerator);
}

float WorldGen::valueAt(int32_t x)
{
	return m_values[x % (VALUES_WIDTH * VALUES_HEIGHT)];
}

float WorldGen::valueAt(int32_t x, int32_t y)
{
	return m_values[(y * VALUES_WIDTH + x) % (VALUES_WIDTH * VALUES_HEIGHT)];
}

float WorldGen::noiseAt(float x)
{
	//std::cout << floor(x) << ' ' << floor(x) + 1 << ' ' << x - floor(x) << '\n';
	return lerp(valueAt(floor(x)), valueAt(floor(x) + 1), x - floor(x));
}

//float WorldGen::noiseAt(float x, float y)
//{
//
//}