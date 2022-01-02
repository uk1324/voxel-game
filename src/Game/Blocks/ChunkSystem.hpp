#pragma once

#include <Math/Vec3.hpp>
#include <Game/Blocks/Chunk.hpp>
#include <Game/Blocks/BlockData.hpp>
#include "PerlinNoise.hpp"
#include <Engine/Graphics/VertexArray.hpp>

#include <unordered_map>

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

	void set(int32_t x, int32_t y, int32_t z, Block block);
	Block get(int32_t x, int32_t y, int32_t z) const;

	void remesh();

private:
	void initializeChunk(Chunk& chunk, const Vec3I& pos);	
	void meshChunk(ChunkData& chunk);

public:
	static constexpr int32_t VERTICAL_RENDER_DISTANCE = 1;
	static constexpr int32_t HORIZONTAL_RENDER_DISTANCE = 1;
	static constexpr int32_t CHUNKS_IN_RENDER_DISTANCE = (2 * VERTICAL_RENDER_DISTANCE + 1) * (2 * HORIZONTAL_RENDER_DISTANCE + 1) * (2 * HORIZONTAL_RENDER_DISTANCE + 1);
	static constexpr size_t VERTEX_DATA_PER_CHUNK_BYTE_SIZE = Chunk::SIZE_CUBED * 6 * 3 * 2 * 4;
	static constexpr size_t VERTEX_DATA_BYTE_SIZE = CHUNKS_IN_RENDER_DISTANCE * VERTEX_DATA_PER_CHUNK_BYTE_SIZE;

private:
	using Vertex = uint32_t;

public:
	PerlinNoise noise;

	Gfx::VertexArray m_vao;
	Gfx::VertexBuffer m_vbo;

	Vec3I m_lastChunkPos;

	std::unordered_map<Vec3I, ChunkData*> m_chunks;
	// Chunk pool shouldn't be modified because other vectors store pointers to it's items. So to modify it you also have to 
	// update all the other vectors.
	std::vector<ChunkData> m_chunkPool;
	std::vector<ChunkData*> m_freeChunks;
	std::vector<ChunkData*> m_chunksToGenerate;
	std::vector<ChunkData*> m_chunksToDraw;

	void addVertex(std::vector<GLuint>& vertices, size_t x, size_t y, size_t z, Block textureIndex, size_t texturePosIndex);
	void addCubeTop(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeBottom(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeLeft(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeRight(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeFront(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeBack(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	bool isInBounds(size_t x, size_t y, size_t z);
	std::vector<uint32_t>& meshFromChunk(Chunk& chunk);

	BlockData blockData;
};