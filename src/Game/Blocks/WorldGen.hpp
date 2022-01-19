#pragma once

#include <Game/Blocks/Chunk.hpp>
#include <Game/Blocks/PerlinNoise.hpp>

#include <array>

#include <imgui.h>

struct Structure
{
	int32_t width, height, depth;
	Block data[5][7][5];
};

class WorldGen
{
public:
	WorldGen(unsigned int seed);

	void generateChunk(Chunk& chunk, const Vec3I& chunkPos);

	void updateTools();

private:
	float overhangNoise(const Vec3& pos);
	float heightMapNoise(const Vec2& posXz);

	float overhangNoiseLimit();
	float heightMapNoiseLimit();

	float hash(const Vec2I& vec);
	std::vector<Vec3I> getTreePositions(const Vec3I& chunkPos);
	int32_t findGroundY(const Vec2& posXz);

	float maxHeight();
	float minHeight();

private:
	float heightMapScale = 0.05f;
	float heightMapAmplitude = 48.0f;
	int heightMapOctaves = 4;
	float heightMapOctaveLacunarity = 0.934f;
	float heightMapOctavePersitence = 0.191f;

	float noiseScale = 0.031f;
	int   noiseOctaves = 2;
	float noiseOctaveLacunarity = 1.179f;
	float noiseOctavePersitence = 0.5f;
	float noiseValueScale = 5.000f;

private:
	PerlinNoise m_noise;
};
