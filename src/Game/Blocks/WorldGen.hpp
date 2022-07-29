#pragma once

#include <Game/Blocks/Chunk.hpp>
#include <Game/Blocks/PerlinNoise.hpp>
#include <Game/Blocks/Structure.hpp>

#include <array>


class WorldGen
{
public:
	WorldGen(unsigned int seed);

	void generateChunk(Chunk& chunk, const Vec3I& chunkPos) const;

	void updateDebugConfig();

private:
	float overhangNoise(const Vec3& pos) const;
	float heightMapNoise(const Vec2& posXz) const;

	float terrainNoise(float heightMapNoiseValue, const Vec3& pos) const;

	float hash(const Vec2I& vec) const;

	std::vector<Structure> getStructures(const Vec3I& chunkPos) const;
	int32_t findGroundY(const Vec2& posXz) const;

	float maxHeight() const;
	float minHeight() const;

private:
	int32_t waterLevel = Chunk::SIZE_Y / 3;

	float heightMapScale = 0.05f;
	int heightMapOctaves = 4;
	float heightMapOctaveLacunarity = 0.934f;
	float heightMapOctavePersitence = 0.191f;

	float overhangNoiseScale = 0.031f;
	int overhangNoiseOctaves = 2;
	float overhangNoiseOctaveLacunarity = 1.179f;
	float overhangNoiseOctavePersitence = 0.5f;

	float terrainAmplitude = 144.0f;

	float stoneDepth = 4.0f;
	float heightOffset = int32_t(Chunk::SIZE_Y / 2);

	float treeSpawnChance = 0.01f;

private:
	PerlinNoise m_noise;

	StructureData m_tree;
	StructureData m_flower;
};
