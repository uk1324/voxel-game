#pragma once

#include <Math/Vec3.hpp>
#include <Game/Blocks/Chunk.hpp>
#include "PerlinNoise.hpp"
#include <Engine/Graphics/VertexArray.hpp>

#include <unordered_map>
#include <memory>


struct ChunkStruct
{
	VertexArray vao;
	VertexBuffer vbo;
	size_t vertexCount;
};

namespace std {
	template <> struct hash<Vec3I>
	{
		size_t operator()(const Vec3I& vec) const
		{
			static constexpr uint64_t SIZE = 2 ^ 12;
			return (vec.x * SIZE * SIZE) + (vec.y * SIZE) + (vec.z);
		}
	};
}

using namespace siv;

class ChunkSystem
{
public:
	ChunkSystem();

	void update(const Vec3& position);

private:
	void initializeChunk(Chunk* chunk, const Vec3I& pos);
	
	void meshChunk(const Vec3I& pos);

public:
	static constexpr int VERTICAL_RENDER_DISTANCE = 5;
	static constexpr int HORIZONTAL_RENDER_DISTANCE = 5;

public:
	PerlinNoise noise;

	Vec3I lastPos;

	std::unordered_map<Vec3I, ChunkStruct> chunkMesh;

	// Iterate in a spiral so no storing is needed
	// Sort based on distance.
	std::vector<Vec3I> chunksToGenerate;
	std::vector<Vec3I> chunkToMesh;

	// Implement lazy chunk loading. How to mesh the chunks not knowing the rest of the chunks. Generate the outside of every chunk??
	// Maybe don't load chunks behind the player
	// Find a better way to check what chunks need to be generated. This probably isn't a bottleneck though.

	std::vector<std::unique_ptr<Chunk>> chunkPool;
	std::vector<Chunk*> freeChunks;
	std::unordered_map<Vec3I, Chunk*> chunks;
	std::vector<Vec3I> deletedChunks;
};