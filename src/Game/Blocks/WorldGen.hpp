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
	float shapeNoise(const Vec3& p);

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

	float hh = 5.0f;


private:
	PerlinNoise m_noise;
};
