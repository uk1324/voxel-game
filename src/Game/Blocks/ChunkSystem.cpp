#include <Game/Blocks/ChunkSystem.hpp>
#include <Utils/Assertions.hpp>

#include <glad/glad.h>

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

static void addVertex(std::vector<GLuint>& vertices, size_t x, size_t y, size_t z, Block textureIndex, size_t texturePosIndex)
{
	GLuint vertex;

	static constexpr int Y_OFFSET = 5;
	static constexpr int Z_OFFSET = Y_OFFSET + 5;
	static constexpr int TEXTURE_INDEX_OFFSET = Z_OFFSET + 5;
	static constexpr int TEXTURE_POS_INDEX_OFFSET = TEXTURE_INDEX_OFFSET + 10;

	vertex = x;
	vertex |= (y << Y_OFFSET);
	vertex |= (z << Z_OFFSET);
	vertex |= ((static_cast<GLuint>(textureIndex.type) - 1) << TEXTURE_INDEX_OFFSET);
	vertex |= (texturePosIndex << TEXTURE_POS_INDEX_OFFSET);

	vertices.push_back(vertex);
}

static void addCubeTop(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y + 1, z, block, 0);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 3);
	addVertex(vertices, x, y + 1, z + 1, block, 2);

	addVertex(vertices, x, y + 1, z, block, 0);
	addVertex(vertices, x + 1, y + 1, z, block, 1);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 3);
}

static void addCubeBottom(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y, z, block, 0);
	addVertex(vertices, x, y, z + 1, block, 2);
	addVertex(vertices, x + 1, y, z + 1, block, 3);

	addVertex(vertices, x, y, z, block, 0);
	addVertex(vertices, x + 1, y, z + 1, block, 3);
	addVertex(vertices, x + 1, y, z, block, 1);
}

static void addCubeLeft(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y, z, block, 0);
	addVertex(vertices, x, y + 1, z, block, 2);
	addVertex(vertices, x, y, z + 1, block, 1);

	addVertex(vertices, x, y, z + 1, block, 1);
	addVertex(vertices, x, y + 1, z, block, 2);
	addVertex(vertices, x, y + 1, z + 1, block, 3);
}

static void addCubeRight(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x + 1, y, z, block, 0);
	addVertex(vertices, x + 1, y, z + 1, block, 1);
	addVertex(vertices, x + 1, y + 1, z, block, 2);

	addVertex(vertices, x + 1, y, z + 1, block, 1);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 3);
	addVertex(vertices, x + 1, y + 1, z, block, 2);
}

static void addCubeFront(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y, z, block, 0);
	addVertex(vertices, x + 1, y + 1, z, block, 3);
	addVertex(vertices, x, y + 1, z, block, 2);

	addVertex(vertices, x, y, z, block, 0);
	addVertex(vertices, x + 1, y, z, block, 1);
	addVertex(vertices, x + 1, y + 1, z, block, 3);
}

static void addCubeBack(Block block, std::vector<GLuint>& vertices, size_t x, size_t y, size_t z)
{
	addVertex(vertices, x, y, z + 1, block, 0);
	addVertex(vertices, x, y + 1, z + 1, block, 2);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 3);

	addVertex(vertices, x, y, z + 1, block, 0);
	addVertex(vertices, x + 1, y + 1, z + 1, block, 3);
	addVertex(vertices, x + 1, y, z + 1, block, 1);
}

static bool isInBounds(size_t x, size_t y, size_t z)
{
	return ((x >= 0) && (x < Chunk::SIZE))
		&& ((y >= 0) && (y < Chunk::SIZE))
		&& ((z >= 0) && (z < Chunk::SIZE));
}

static std::vector<uint32_t>& meshFromChunk(Chunk& chunk)
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
						addCubeTop(block, vertices, x, y, z);
					}
					if (isInBounds(x, y - 1, z) == false || chunk(x, y - 1, z) == BlockType::Air)
					{
						addCubeBottom(block, vertices, x, y, z);
					}
					if (isInBounds(x + 1, y, z) == false || chunk(x + 1, y, z) == BlockType::Air)
					{
						addCubeRight(block, vertices, x, y, z);
					}
					if (isInBounds(x - 1, y, z) == false || chunk(x - 1, y, z) == BlockType::Air)
					{
						addCubeLeft(block, vertices, x, y, z);
					}
					if (isInBounds(x, y, z + 1) == false || chunk(x, y, z + 1) == BlockType::Air)
					{
						addCubeBack(block, vertices, x, y, z);
					}
					if (isInBounds(x, y, z - 1) == false || chunk(x, y, z - 1) == BlockType::Air)
					{
						addCubeFront(block, vertices, x, y, z);
					}
				}
			}
		}
	}

	return vertices;
}

#include <iostream>
#include <unordered_set>

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

void ChunkSystem::set(int32_t x, int32_t y, int32_t z, Block block)
{
	Vec3I chunkPos(floor(float(x) / Chunk::SIZE), floor(float(y) / Chunk::SIZE), floor(float(z) / Chunk::SIZE));
	Vec3I posInChunk(x - chunkPos.x * Chunk::SIZE, y - chunkPos.y * Chunk::SIZE, z - chunkPos.z * Chunk::SIZE);
	ASSERT(posInChunk.x >= 0 && posInChunk.y >= 0 && posInChunk.z >= 0);
	std::cout << chunkPos << ' ' << posInChunk << '\n';
	ChunkData& chunk = *m_chunks[chunkPos];

	auto find = [this](const Vec3I& chunkPos) -> ChunkData&
	{
		auto a = m_chunks.find(chunkPos);
		//auto a =std::find_if(m_chunksToDraw.begin(), m_chunksToDraw.end(), [&chunkPos, this](ChunkData* a) { if (a->pos == chunkPos) { m_freeChunks.push_back(a); return true; } else return false; });
		ASSERT(a != m_chunks.end());
		return *a->second;
	};

	//for (block)

	chunk.blocks.set(posInChunk.x, posInChunk.y, posInChunk.z, block);
	auto& a = find(chunkPos);
	ASSERT(&a == &chunk); 
	//remesh();
	meshChunk(chunk);


	//std::cout << "pos: " << Vec3I(x, y, z) << " | " << "chunkPos: " << chunkPos << " | " << "posInChunk: " << posInChunk << '\n';
	if (posInChunk.x >= 0)
	{
		if (posInChunk.x == 0)
		{
			//ASSERT_NOT_REACHED();
			meshChunk(find(chunkPos + Vec3I(-1, 0, 0)));
		} 
		if (posInChunk.x == Chunk::SIZE - 1)
		{
			//ASSERT_NOT_REACHED();
			meshChunk(find(chunkPos + Vec3I(1, 0, 0)));
		}
	}
	else
	{
		//ASSERT_NOT_REACHED();
		if (posInChunk.x == -(int32_t(Chunk::SIZE) - 1))
		{
			meshChunk(find(chunkPos + Vec3I(-1, 0, 0)));
		}
		if (posInChunk.x == -1)
		{
			meshChunk(find(chunkPos + Vec3I(1, 0, 0)));
		}
	}
	
	if (posInChunk.y >= 0)
	{
		if (posInChunk.y == 0)
		{
			//ASSERT_NOT_REACHED();
			//std::cout << "c\n";
			meshChunk(find(chunkPos + Vec3I(0, -1, 0)));
		}
		if (posInChunk.y == Chunk::SIZE - 1)
		{
			//ASSERT_NOT_REACHED();
			//std::cout << "d\n";
			meshChunk(find(chunkPos + Vec3I(0, 1, 0)));
		}
	}
	else
	{
		//ASSERT_NOT_REACHED();
		if (posInChunk.y == -(int32_t(Chunk::SIZE) - 1))
		{
			meshChunk(find(chunkPos + Vec3I(-1, 0, 0)));
		}
		if (posInChunk.y == -1)
		{
			meshChunk(find(chunkPos + Vec3I(1, 0, 0)));
		}
	}
	if (posInChunk.z >= 0)
	{
		if (posInChunk.z == 0)
		{
			//std::cout << "e\n";
			//ASSERT_NOT_REACHED();
			meshChunk(find(chunkPos + Vec3I(0, 0, -1)));
		}
		if (posInChunk.z == Chunk::SIZE - 1)
		{
			//ASSERT_NOT_REACHED();
			//std::cout << "f\n";
			meshChunk(find(chunkPos + Vec3I(0, 0, 1)));
		}
	}
	else
	{
		//ASSERT_NOT_REACHED();
		if (posInChunk.z == -(int32_t(Chunk::SIZE) - 1))
		{
			meshChunk(find(chunkPos + Vec3I(-1, 0, 0)));
		}
		if (posInChunk.z == -1)
		{
			meshChunk(find(chunkPos + Vec3I(1, 0, 0)));
		}
	}


}

Block ChunkSystem::get(int32_t x, int32_t y, int32_t z) const
{
	Vec3I chunkPos(floor(float(x) / Chunk::SIZE), floor(float(y) / Chunk::SIZE), floor(float(z) / Chunk::SIZE));
	Vec3I posInChunk(x - chunkPos.x * Chunk::SIZE, y - chunkPos.y * Chunk::SIZE, z - chunkPos.z * Chunk::SIZE);
	//if (posInChunk)
	//if (posInChunk.x < 0)
	//{
	//	posInChunk.x += Chunk::SIZE;
	//}
	//if (posInChunk.y < 0)
	//{
	//	posInChunk.y += Chunk::SIZE;
	//}
	//if (posInChunk.z < 0)
	//{
	//	posInChunk.z += Chunk::SIZE;
	//}

	auto it = m_chunks.find(chunkPos);
	if (it == m_chunks.end())
		return BlockType::Air;

	/*if (it->second->blocks.get(posInChunk.x, posInChunk.y, posInChunk.z).isSolid())
		std::cout << "get: " << chunkPos << ' ' << posInChunk << '\n';*/

	return it->second->blocks.get(posInChunk.x, posInChunk.y, posInChunk.z);
}

void ChunkSystem::remesh()
{
	meshChunk(*m_chunks[Vec3I(62, 1, 62)]);
	//for (auto chunk : m_chunksToDraw)
	//{
	//	meshChunk(*chunk);
	//}
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
			for (size_t y = 0; y < Chunk::SIZE; y++)
			{
				//double value = noise.accumulatedOctaveNoise3D_0_1((x + pos.x * 16.0) / 20.0, (y + pos.y * 16.0) / 20.0, (z + pos.z * 16.0) / 20.0, 8);
				if (x == 8 && y == 8 && z == 8)
				{
					chunk.operator()(x, y, z) = BlockType::Grass2;
				}
				if (value > 0.4)
				{
					chunk.operator()(x, y, z) = BlockType::Grass2;
				}
				if ((y + pos.y * Chunk::SIZE) < value)
				{
					//chunk->operator()(x, y, z) = static_cast<BlockType>((rand() % 3) + 1);
					/*chunk->operator()(x, y, z) = static_cast<BlockType>((y % 3) + 1);*/
					int realY = y + pos.y * Chunk::SIZE;
					if (realY > 30)
					{
						chunk.operator()(x, y, z) = BlockType::Grass2;
					}
					else if (realY < 30 && realY > 25)
					{
						if (rand() % 2 == 0)
						{
							chunk.operator()(x, y, z) = BlockType::Grass2;
						}
						else
						{
							chunk.operator()(x, y, z) = BlockType::Grass;
						}
					}
					else
					{
						chunk.operator()(x, y, z) = BlockType::Grass;
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
	//std::cout << vertices.size() << ' ' << chunk.vboByteOffset << '\n';
	//std::cout << "meshing chunk: " << chunk.pos << '\n';
	//std::cout << "meshing: " << chunk.pos << '\n';
	chunk.vertexCount = vertices.size();
	m_vbo.bind();
	m_vbo.setData(chunk.vboByteOffset, vertices.data(), vertices.size() * sizeof(uint32_t));
}
