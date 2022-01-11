#include <Game/Blocks/ChunkSystem.hpp>
#include <Utils/Assertions.hpp>
#include <glad/glad.h>

#include <unordered_set>

ChunkSystem::ChunkSystem()
// lastPos is infinity so all the chunks get generated at the start
	: m_lastChunkPos(Vec3I(std::numeric_limits<int32_t>::infinity(), 0, 0))
	, noise()
	// Preallocating all the vertex data isn't a good idea for open worlds because a lot of space if taken up by empty chunks
	// but it makes sense for cave worlds where the render distance is smaller.
	, m_vbo(VERTEX_DATA_BYTE_SIZE)
{
	m_chunkPool.resize(CHUNKS_IN_RENDER_DISTANCE);
	m_freeChunks.reserve(CHUNKS_IN_RENDER_DISTANCE);
	for (size_t i = 0; i < m_chunkPool.size(); i++)
	{
		m_chunkPool[i].vboByteOffset = VERTEX_DATA_PER_CHUNK_BYTE_SIZE * i;
		m_freeChunks.push_back(&m_chunkPool[i]);
	}

	m_vao.bind();
	m_vao.setAttribute(0, Gfx::IntBufferLayout(Gfx::ShaderDataType::UnsignedInt, 1, 0, 4));
}

void ChunkSystem::addVertex(std::vector<GLuint>& vertices, size_t x, size_t y, size_t z, Block textureIndex, size_t texturePosIndex)
{
	GLuint vertex;

	static constexpr int Y_OFFSET = 5;
	static constexpr int Z_OFFSET = Y_OFFSET + 5;
	static constexpr int TEXTURE_INDEX_OFFSET = Z_OFFSET + 5;
	static constexpr int TEXTURE_POS_INDEX_OFFSET = TEXTURE_INDEX_OFFSET + 10;

	vertex = x;
	vertex |= (y << Y_OFFSET);
	vertex |= (z << Z_OFFSET);
	vertex |= ((static_cast<GLuint>(textureIndex.type)) << TEXTURE_INDEX_OFFSET);
	vertex |= (texturePosIndex << TEXTURE_POS_INDEX_OFFSET);

	vertices.push_back(vertex);
}

// To swap texture rotation 
// 0 = vec2(0, 0) // bottom left
// 1 = vec2(1, 0) // bottom right
// 2 = vec2(0, 1) // top left
// 3 = vec2(1, 1) // top right
// 90deg swap 0 with 3 or 2 with 1
// 180 swap 0 with 3 and 2 with 1
// to flip upside down swap 0 with 2 and 1 with 3
// to mirror swap 0 with 1 and 2 with 3
void ChunkSystem::addCubeTop(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y + 1, z, block, 3);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 0);
	addVertex(vertices, x, y + 1, z + 1, block, 1);

	addVertex(vertices, x, y + 1, z, block, 3);
	addVertex(vertices, x + 1, y + 1, z, block, 2);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 0);
}

void ChunkSystem::addCubeBottom(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y, z, block, 0);
	addVertex(vertices, x, y, z + 1, block, 2);
	addVertex(vertices, x + 1, y, z + 1, block, 3);

	addVertex(vertices, x, y, z, block, 0);
	addVertex(vertices, x + 1, y, z + 1, block, 3);
	addVertex(vertices, x + 1, y, z, block, 1);
}

void ChunkSystem::addCubeLeft(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y, z, block, 2);
	addVertex(vertices, x, y + 1, z, block, 0);
	addVertex(vertices, x, y, z + 1, block, 3);

	addVertex(vertices, x, y, z + 1, block, 3);
	addVertex(vertices, x, y + 1, z, block, 0);
	addVertex(vertices, x, y + 1, z + 1, block, 1);
}

void ChunkSystem::addCubeRight(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x + 1, y, z, block, 3);
	addVertex(vertices, x + 1, y, z + 1, block, 2);
	addVertex(vertices, x + 1, y + 1, z, block, 1);

	addVertex(vertices, x + 1, y, z + 1, block, 2);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 0);
	addVertex(vertices, x + 1, y + 1, z, block, 1);
}

void ChunkSystem::addCubeFront(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y, z, block, 3);
	addVertex(vertices, x + 1, y + 1, z, block, 0);
	addVertex(vertices, x, y + 1, z, block, 1);

	addVertex(vertices, x, y, z, block, 3);
	addVertex(vertices, x + 1, y, z, block, 2);
	addVertex(vertices, x + 1, y + 1, z, block, 0);
}

void ChunkSystem::addCubeBack(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y, z + 1, block, 2);
	addVertex(vertices, x, y + 1, z + 1, block, 0);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 1);

	addVertex(vertices, x, y, z + 1, block, 2);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 1);
	addVertex(vertices, x + 1, y, z + 1, block, 3);
}

bool ChunkSystem::isInBounds(size_t x, size_t y, size_t z)
{
	return ((x >= 0) && (x < Chunk::SIZE))
		&& ((y >= 0) && (y < Chunk::SIZE))
		&& ((z >= 0) && (z < Chunk::SIZE));
}

ChunkSystem::Pos ChunkSystem::posToChunkPosAndPosInChunk(const Vec3& pos)
{
	Pos block;
	block.chunkPos = Vec3I((pos / Chunk::SIZE).applied(floor));
	block.posInChunk = Vec3I(pos.applied(floor) - Vec3(block.chunkPos * Chunk::SIZE));
	return block;
}

ChunkSystem::Pos ChunkSystem::posToChunkPosAndPosInChunk(const Vec3I& pos)
{
	Pos block;
	block.chunkPos = Vec3I((Vec3(pos) / Chunk::SIZE).applied(floor));
	block.posInChunk = Vec3I(Vec3(pos).applied(floor) - Vec3(block.chunkPos * Chunk::SIZE));
	return block;
}

std::vector<uint32_t>& ChunkSystem::meshFromChunk(Chunk& chunk)
{
	static std::vector<uint32_t> vertices(16 * 16 * 16 * 6 * 3);
	vertices.clear();
	//vertices.reserve(16 * 16 * 16 * 6 * 3);

	for (size_t z = 0; z < Chunk::SIZE; z++)
	{
		for (size_t y = 0; y < Chunk::SIZE; y++)
		{
			for (size_t x = 0; x < Chunk::SIZE; x++)
			{
				if (chunk(x, y, z).type != BlockType::Air)
				{
					Block block = chunk(x, y, z);
					if (isInBounds(x, y + 1, z) == false || chunk(x, y + 1, z) == BlockType::Air)
					{
						addCubeTop(static_cast<BlockType>(blockData[block.type].topTextureIndex), vertices, x, y, z);
					}
					if (isInBounds(x, y - 1, z) == false || chunk(x, y - 1, z) == BlockType::Air)
					{
						addCubeBottom(static_cast<BlockType>(blockData[block.type].bottomTextureIndex), vertices, x, y, z);
					}
					if (isInBounds(x + 1, y, z) == false || chunk(x + 1, y, z) == BlockType::Air)
					{
						addCubeRight(static_cast<BlockType>(blockData[block.type].rightTextureIndex), vertices, x, y, z);
					}
					if (isInBounds(x - 1, y, z) == false || chunk(x - 1, y, z) == BlockType::Air)
					{
						addCubeLeft(static_cast<BlockType>(blockData[block.type].leftTextureIndex), vertices, x, y, z);
					}
					if (isInBounds(x, y, z + 1) == false || chunk(x, y, z + 1) == BlockType::Air)
					{
						addCubeBack(static_cast<BlockType>(blockData[block.type].backTextureIndex), vertices, x, y, z);
					}
					if (isInBounds(x, y, z - 1) == false || chunk(x, y, z - 1) == BlockType::Air)
					{
						addCubeFront(static_cast<BlockType>(blockData[block.type].frontTextureIndex), vertices, x, y, z);
					}
				}
			}
		}
	}

	return vertices;
}

void ChunkSystem::update(const Vec3& loadPos)
{
	if (m_chunksToGenerate.size() == CHUNKS_IN_RENDER_DISTANCE)
	{

	}
	// generate all

	if (m_chunksToGenerate.size() != 0)
	{
		ChunkData& chunk = *m_chunksToGenerate.back();
	
		initializeChunk(chunk.blocks, chunk.pos);
		meshChunk(chunk);
		m_chunksToDraw.push_back(&chunk);
		m_chunks[chunk.pos] = &chunk;
		m_chunksToGenerate.pop_back();
	}

	const Vec3I chunkPos(floor(loadPos.x / Chunk::SIZE), floor(loadPos.y / Chunk::SIZE), floor(loadPos.z / Chunk::SIZE));

	if (chunkPos == m_lastChunkPos)
	{
		m_lastChunkPos = chunkPos;
		return;
	}
	m_lastChunkPos = chunkPos;

	auto isChunkOutOfRenderDistance = [&chunkPos](const Vec3I& pos)
	{
		return (pos.x < (chunkPos.x - HORIZONTAL_RENDER_DISTANCE)) || (pos.x > (chunkPos.x + HORIZONTAL_RENDER_DISTANCE))
			|| (pos.y < (chunkPos.y - VERTICAL_RENDER_DISTANCE))   || (pos.y > (chunkPos.y + VERTICAL_RENDER_DISTANCE))
			|| (pos.z < (chunkPos.z - HORIZONTAL_RENDER_DISTANCE)) || (pos.z > (chunkPos.z + HORIZONTAL_RENDER_DISTANCE));
	};

	m_chunksToGenerate.erase(
		std::remove_if(
			m_chunksToGenerate.begin(),
			m_chunksToGenerate.end(),
			[isChunkOutOfRenderDistance, this](ChunkData* chunk)
			{
				if (isChunkOutOfRenderDistance(chunk->pos))
				{
					m_freeChunks.push_back(chunk);
					return true;
				}
				else
				{
					return false;
				}
			}
		),
		m_chunksToGenerate.end()
	);
	m_chunksToDraw.erase(
		std::remove_if(
			m_chunksToDraw.begin(),
			m_chunksToDraw.end(), 
			[isChunkOutOfRenderDistance, this](ChunkData* chunk) {
				if (isChunkOutOfRenderDistance(chunk->pos))
				{
					m_chunks.erase(chunk->pos);
					m_freeChunks.push_back(chunk);
					return true;
				}
				else
				{
					return false;
				}
			}
		),
		m_chunksToDraw.end()
	);


	std::unordered_set<Vec3I> chunksToGenerateSet;
	for (auto a : m_chunksToGenerate)
	{
		chunksToGenerateSet.insert(a->pos);
	}

	for (int32_t z = chunkPos.z - HORIZONTAL_RENDER_DISTANCE; z <= chunkPos.z + HORIZONTAL_RENDER_DISTANCE; z++)
	{
		for (int32_t y = chunkPos.y - VERTICAL_RENDER_DISTANCE; y <= chunkPos.y + VERTICAL_RENDER_DISTANCE; y++)
		{
			for (int32_t x = chunkPos.x - HORIZONTAL_RENDER_DISTANCE; x <= chunkPos.x + HORIZONTAL_RENDER_DISTANCE; x++)
			{
				const Vec3I chunkPos(x, y, z);
				const auto chunk = m_chunks.find(chunkPos);

				if (chunk == m_chunks.end()
  				 && chunksToGenerateSet.find(chunkPos) == chunksToGenerateSet.end())
				{
					ASSERT(m_freeChunks.size() != 0 && "Chunk pool should have enough chunks");
					ChunkData& chunk = *m_freeChunks.back();
					m_freeChunks.pop_back();
					chunk.pos = chunkPos;
					m_chunksToGenerate.push_back(&chunk);
				}
			}
		}
	}

	std::sort(m_chunksToGenerate.begin(), m_chunksToGenerate.end(),
		[&chunkPos](const ChunkData* a, const ChunkData* b) {
			return Vec3I::distanceSquared(a->pos, chunkPos) > Vec3I::distanceSquared(b->pos, chunkPos);
		}
	); 
}

void ChunkSystem::trySetBlock(const Vec3I& blockPos, Block block)
{
	const auto pos = posToChunkPosAndPosInChunk(blockPos);

	auto optChunk = tryGet(pos.chunkPos);
	if (optChunk.has_value() == false)
		return;
	ChunkData& chunk = *optChunk.value();

	chunk.blocks.set(pos.posInChunk, block);
	meshChunk(chunk);
	remeshChunksAround(pos);
}

Opt<Block> ChunkSystem::tryGetBlock(const Vec3I& blockPos) const
{
	const auto pos = posToChunkPosAndPosInChunk(blockPos);

	const auto optChunk = tryGet(pos.chunkPos);
	if (optChunk.has_value() == false)
		return std::nullopt;

	return optChunk.value()->blocks.get(pos.posInChunk);
}

Opt<ChunkData*> ChunkSystem::tryGet(const Vec3I& pos)
{
	auto chunk = m_chunks.find(pos);
	if (chunk == m_chunks.end())
		return std::nullopt;
	return chunk->second;
}

Opt<const ChunkData*> ChunkSystem::tryGet(const Vec3I& pos) const
{
	auto chunk = m_chunks.find(pos);
	if (chunk == m_chunks.end())
		return std::nullopt;
	return chunk->second;
}

void ChunkSystem::remeshChunksAround(const Pos& pos)
{
	const auto& [chunkPos, posInChunk] = pos;

	if (posInChunk.x >= 0)
	{
		if (posInChunk.x == 0)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(-1, 0, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
		if (pos.posInChunk.x == Chunk::SIZE - 1)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(1, 0, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
	}
	else
	{
		if (posInChunk.x == (-static_cast<int32_t>(Chunk::SIZE) - 1))
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(-1, 0, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
		if (posInChunk.x == -1)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(1, 0, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
	}

	if (posInChunk.y >= 0)
	{
		if (posInChunk.y == 0)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(0, -1, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
		if (posInChunk.y == Chunk::SIZE - 1)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(0, 1, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
	}
	else
	{
		if (posInChunk.y == (-static_cast<int32_t>(Chunk::SIZE) - 1))
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(-1, 0, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
		if (posInChunk.y == -1)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(1, 0, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
	}
	if (posInChunk.z >= 0)
	{
		if (posInChunk.z == 0)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(0, 0, -1));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
		if (posInChunk.z == Chunk::SIZE - 1)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(0, 0, 1));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
	}
	else
	{
		if (posInChunk.z == -(static_cast<int32_t>(Chunk::SIZE) - 1))
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(-1, 0, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
		if (posInChunk.z == -1)
		{
			auto chunk = tryGet(pos.chunkPos + Vec3I(1, 0, 0));
			if (chunk.has_value())
				meshChunk(*chunk.value());
		}
	}
}

void ChunkSystem::initializeChunk(Chunk& chunk, const Vec3I& pos)
{
	for (size_t z = 0; z < Chunk::SIZE; z++)
	{ 
		for (size_t x = 0; x < Chunk::SIZE; x++)
		{
			double value = noise.accumulatedOctaveNoise2D_0_1((x + pos.x * 16.0) / 256.0, (z + pos.z * 16.0) / 256.0, 8) * 50;
			//double value = 0.2;
			double input = (rand() % 256) / 256.0;

			size_t highest = 0;
			for (size_t y = 0; y < Chunk::SIZE; y++)
			{
				//double value = noise.accumulatedOctaveNoise3D_0_1((x + pos.x * 16.0) / 20.0, (y + pos.y * 16.0) / 20.0, (z + pos.z * 16.0) / 20.0, 8);
				//if (x == 8 && y == 8 && z == 8)
				//{
				//	chunk.operator()(x, y, z) = BlockType::Grass2;
				//}
				if (value > 0.4)
				{
					chunk.operator()(x, y, z) = BlockType::Stone;
				}
				if ((y + pos.y * Chunk::SIZE) < value)
				{
					if ((y + pos.y * Chunk::SIZE + 1) > value)
					{
						chunk.operator()(x, y, z) = BlockType::Grass;
						continue;
					}
					//chunk->operator()(x, y, z) = static_cast<BlockType>((rand() % 3) + 1);
					/*chunk->operator()(x, y, z) = static_cast<BlockType>((y % 3) + 1);*/
					int realY = y + pos.y * Chunk::SIZE;
					if (realY > 30)
					{
						chunk.operator()(x, y, z) = BlockType::Stone;
					}
					else if (realY < 30 && realY > 25)
					{
						if (rand() % 2 == 0)
						{
							chunk.operator()(x, y, z) = BlockType::Stone;
						}
						else
						{
							chunk.operator()(x, y, z) = BlockType::Dirt;
						}
					}
					else
					{
						chunk.operator()(x, y, z) = BlockType::Dirt;
					}
				}
				else
				{
					chunk.operator()(x, y, z) = BlockType::Air;
				}
			}
		}
	}
}

void ChunkSystem::meshChunk(ChunkData& chunk)
{
	const std::vector<uint32_t>& vertices = meshFromChunk(chunk.blocks);
	chunk.vertexCount = vertices.size();
	m_vbo.bind();
	m_vbo.setData(chunk.vboByteOffset, vertices.data(), vertices.size() * sizeof(uint32_t));
}
