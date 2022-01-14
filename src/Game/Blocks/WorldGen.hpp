#pragma once

#include <Game/Blocks/Chunk.hpp>

#include <random>
#include <array>

class WorldGen
{
public:
	WorldGen(unsigned int seed);

	void generateChunk(Chunk& chunk, const Vec3I& chunkPos);

private:
	float randomFloat();
	float valueAt(int32_t x);
	float valueAt(int32_t x, int32_t y);
	float noiseAt(float x);
	float noiseAt(float x, float y);

private:
	template<typename T>
	T lerp(T a, T b, float t);

private:
	static constexpr size_t VALUES_WIDTH = 16;
	static constexpr size_t VALUES_HEIGHT = 16;

private:
	std::array<float, VALUES_WIDTH * VALUES_HEIGHT> m_values;

	std::minstd_rand0 m_randomGenerator;
	std::uniform_real_distribution<float> m_randomDistribution;
};

template<typename T>
inline T WorldGen::lerp(T a, T b, float t)
{
	return (a * (T(1) - t)) + (b * t);
}
