#pragma once

#include <Math/Vec3.hpp>
#include <Game/Blocks/Chunk.hpp>
#include "PerlinNoise.hpp"
#include <Engine/Graphics/VertexArray.hpp>

#include <unordered_map>
#include <memory>


struct ChunkData
{
	//size_t vertexCount;
	//size_t offset;
	//size_t count;
	Chunk blocks;
	size_t vertexCount;
	// Make this const
	size_t vboByteOffset;
	Vec3I pos;
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

	void update(const Vec3& loadPos);

private:
	void initializeChunk(Chunk& chunk, const Vec3I& pos);
	
	void meshChunk(const ChunkData& chunk);

public:
	static constexpr int32_t VERTICAL_RENDER_DISTANCE = 2;
	static constexpr int32_t HORIZONTAL_RENDER_DISTANCE = 1;
	static constexpr int32_t CHUNKS_IN_RENDER_DISTANCE = (2 * VERTICAL_RENDER_DISTANCE + 1) * (2 * HORIZONTAL_RENDER_DISTANCE + 1) * (2 * HORIZONTAL_RENDER_DISTANCE + 1);
	static constexpr size_t VERTEX_DATA_PER_CHUNK_BYTE_SIZE = Chunk::SIZE_CUBED * 6 * 3 * 2 * 4;
	static constexpr size_t VERTEX_DATA_BYTE_SIZE = CHUNKS_IN_RENDER_DISTANCE * VERTEX_DATA_PER_CHUNK_BYTE_SIZE;

public:
	PerlinNoise noise;

	Gfx::VertexArray m_vao;
	Gfx::VertexBuffer m_vbo;

	Vec3I m_lastChunkPos;

	std::unordered_map<Vec3I, ChunkData*> m_chunks;
	// Maybe change to unique ptr or something so pointers are stable. Pointers will be stable even if I don't do that because I don't change it
	std::vector<ChunkData> m_chunkPool;

	std::vector<ChunkData*> m_freeChunks;
	std::vector<ChunkData*> m_chunksToGenerate;

	std::vector<ChunkData*> m_chunksToDraw;
private:

	//std::unordered_map<Vec3I, ChunkStruct> chunkMesh;

	// Iterate in a spiral so no storing is needed
	// Sort based on distance.
	//std::vector<Vec3I> chunksToGenerate;
	//std::vector<Vec3I> chunkToMesh;

	// Implement lazy chunk loading. How to mesh the chunks not knowing the rest of the chunks. Generate the outside of every chunk??
	// Maybe don't load chunks behind the player
	// Find a better way to check what chunks need to be generated. This probably isn't a bottleneck though.

	//std::vector<std::unique_ptr<Chunk>> chunkPool;
	//std::vector<Chunk*> freeChunks;
	//std::unordered_map<Vec3I, Chunk*> chunks;
	//std::vector<Vec3I> deletedChunks;
};