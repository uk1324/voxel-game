#pragma once

#include <Math/Vec3.hpp>
#include <Game/Blocks/Chunk.hpp>
#include <Game/Blocks/BlockData.hpp>
#include <Game/Blocks/WorldGen.hpp>
#include <Engine/Graphics/Vao.hpp>
#include <Utils/Opt.hpp>

#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>

struct ChunkData
{
	Chunk blocks;
	// Could store multiple vertex counts and offsets for different height ranges. This would allow for better frustum culling.
	// Merge multiple adjacent chunks. Use indirect draw calls.
	size_t vertexCount;
	size_t vboByteOffset;
	Vec3I pos;
	size_t waterVertexCount;
	size_t waterVertexCapacity;
	Vao waterVao;
	Vbo waterVbo;
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

// Don't know what would be the best way to mesh chunks if not all the chunks around it a loaded yet.
// I could just increase the load distance and only mesh the chunks which have chunk around them loaded.
// I could store the vertices of the chunk in the vector and upload them when a chunk that wasn't loaded gets loaded.
// I could just remesh every chunk around when a chunk gets loaded.

// TODO: Implement some better way for allocating chunk data. Block allocator maybe.
// Don't know how expensive are state changes. The simplest thing would be to just allocate a Vao and Vbo per chunk
// and reallocate a bigger amount if needed.
class ChunkSystem
{
public:
	struct Pos
	{
		Vec3I chunkPos;
		Vec3I posInChunk;
	};

public:
	ChunkSystem();
	~ChunkSystem();

	void update(const Vec3& loadPos);

	void trySetBlock(const Vec3I& blockPos, Block block);
	Opt<Block> tryGetBlock(const Vec3I& blockPos) const;
	Opt<ChunkData*> tryGet(const Vec3I& pos);
	Opt<const ChunkData*> tryGet(const Vec3I& pos) const;

	void remeshChunksAround(const Pos& pos);

	void regenerateAll();
	bool shouldRegenerateAll = false;

private:
	void meshChunk(ChunkData& chunk);

	void generateChunks();

	struct ChunkVertices 
	{
		std::vector<uint32_t>& vertices;
		std::vector<uint32_t>& waterVertices;
	} meshFromChunk(Chunk& chunk, const Vec3I& pos);
	void addVertex(std::vector<GLuint>& vertices, size_t x, size_t y, size_t z, Block textureIndex, size_t texturePosIndex, size_t normalIndex);
	void addCubeTop(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeBottom(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeLeft(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeRight(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeFront(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void addCubeBack(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	void meshDecoration(uint32_t textureIndex, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z);
	bool isInBounds(size_t x, size_t y, size_t z);

public:
	static Pos posToChunkPosAndPosInChunk(const Vec3& pos);
	static Pos posToChunkPosAndPosInChunk(const Vec3I& pos);

public:
	// When using a extendible vertical render distance it makes no sense to create shadows becuase there might 
	// be things above that haven't been loaded yet. A similar issue still happens if the angle of light is very small.
	// Another issue is that if during world generation chunks are split vertically then chunk below and above would need
	// to recompute the vertical noise value. A soultion might be to use a cache, but this isn't without cost. Because multiple
	// threads can access and write to the cache it would need to be locked and also cpu cache ping pong would happen.
	//static constexpr int32_t HORIZONTAL_RENDER_DISTANCE = 4;
	static constexpr int32_t HORIZONTAL_RENDER_DISTANCE = 12;

	static constexpr int32_t CHUNKS_IN_RENDER_DISTANCE = (2 * HORIZONTAL_RENDER_DISTANCE + 1) * (2 * HORIZONTAL_RENDER_DISTANCE + 1);
	static constexpr size_t VERTEX_DATA_PER_CHUNK_BYTE_SIZE = (Chunk::BLOCK_COUNT * 6 * 3 * 2 * 4) * 0.2; // * 0.2 for testing
	// Don't know what is the best way to store vertex data.
	// When stored in a single vao and vbo it takes up too much space because it preallocates for the worst case scenario. 
	// It average it uses only around 3% of the total allocated memory.
	// Could also create an allocator that allocates large buffers and the allocates blocks in those buffers to chunks.
	// This would make chunks need to store a list of buffer locations. To reduce context changes the vertices would need to be storted based on
	// The vao which would prevent optimization like sorting the chunks based on distance to reduce the amount of fragments drawn.
	static constexpr size_t VERTEX_DATA_BYTE_SIZE = CHUNKS_IN_RENDER_DISTANCE * VERTEX_DATA_PER_CHUNK_BYTE_SIZE;

private:
	using Vertex = uint32_t;

public:
	WorldGen m_worldGen;

	Vao m_vao;
	Vbo m_vbo;

	Vec3I m_lastChunkPos;

	std::unordered_map<Vec3I, ChunkData*> m_chunks;
	// Chunk pool shouldn't be modified because other vectors store pointers to it's items. So to modify it you also have to 
	// update all the other vectors.
	std::vector<ChunkData> m_chunkPool;
	std::vector<ChunkData*> m_freeChunks;
	std::vector<ChunkData*> m_chunksToGenerate;
	std::vector<ChunkData*> m_generatedChunks;
	std::vector<ChunkData*> m_chunksToMesh; // Already generated, but not meshed because chunks around it aren't generated yet.
	std::vector<ChunkData*> m_chunksToDraw;
	
	// Already meshed, but got modified.
	std::unordered_set<ChunkData*> m_chunksToRemesh;

	std::mutex mutex;

	std::atomic<bool> m_isFinished;
	std::thread m_worldGenerationThread;

	BlockData blockData;
};