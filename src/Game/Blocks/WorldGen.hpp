#pragma once

#include <Game/Blocks/Chunk.hpp>
#include <Game/Blocks/PerlinNoise.hpp>

#include <array>

#include <imgui.h>

class WorldGen
{
public:
	WorldGen(unsigned int seed);

	void generateChunk(Chunk& chunk, const Vec3I& chunkPos);

	void updateTools();

private:
	float heightMapScale = 0.05f;
	float heightMapAmplitude = 25.0f;
	int heightMapOctaves = 4;
	float heightMapOctaveLacunarity = 0.5f;
	float heightMapOctavePersitence = 0.5f;

	float noiseScale = 0.05f;
	int   noiseOctaves = 4;
	float noiseOctaveLacunarity = 0.5f;
	float noiseOctavePersitence = 0.5f;
	float noiseValueScale = 1.0f;


private:
	PerlinNoise m_noise;
};
