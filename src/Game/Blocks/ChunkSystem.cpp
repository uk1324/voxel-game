#include <Game/Blocks/ChunkSystem.hpp>
#include <Utils/Assertions.hpp>

#include <glad/glad.h>

ChunkSystem::ChunkSystem()
// lastPos is infinity so all the chunks get generated at the start
	: m_lastChunkPos(Vec3I(std::numeric_limits<int32_t>::infinity(), 0, 0))
	, noise()
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

void ChunkSystem::update(const Vec3& loadPos)
{
	// if (m_chunksToGenerate.size() == CHUNKS_IN_RENDER_DISTANCE)
	// generate all

	if (m_chunksToGenerate.size() != 0)
	{
		ChunkData& chunk = *m_chunksToGenerate.back();
	
		initializeChunk(chunk.blocks, chunk.pos);
		std::vector<uint32_t>& vertices = meshFromChunk(chunk.blocks);
		chunk.vertexCount = vertices.size();
		m_vbo.setData(chunk.vboByteOffset, vertices.data(), vertices.size() * sizeof(uint32_t));
		m_chunksToDraw.push_back(&chunk);
		m_chunks[chunk.pos] = &chunk;
		m_chunksToGenerate.pop_back();
	}

	const Vec3I chunkPos(loadPos.x / Chunk::SIZE, loadPos.y / Chunk::SIZE, loadPos.z / Chunk::SIZE);

	if (chunkPos == m_lastChunkPos)
	{
		m_lastChunkPos = chunkPos;
		return;
	}
	m_lastChunkPos = chunkPos;


	auto removeOutOfDistance = [&chunkPos, this](ChunkData* chunk) {
		if ((chunk->pos.x < (chunkPos.x - HORIZONTAL_RENDER_DISTANCE)) || (chunk->pos.x > (chunkPos.x + HORIZONTAL_RENDER_DISTANCE))
		 || (chunk->pos.y < (chunkPos.y - VERTICAL_RENDER_DISTANCE))   || (chunk->pos.y > (chunkPos.y + VERTICAL_RENDER_DISTANCE))
		 || (chunk->pos.z < (chunkPos.z - HORIZONTAL_RENDER_DISTANCE)) || (chunk->pos.z > (chunkPos.z + HORIZONTAL_RENDER_DISTANCE)))
		{
			m_chunks.erase(chunk->pos);
			m_freeChunks.push_back(chunk);
			return true;
		}
		else
		{
			return false;
		}
	};

	m_chunksToGenerate.erase(std::remove_if(m_chunksToGenerate.begin(), m_chunksToGenerate.end(), removeOutOfDistance), m_chunksToGenerate.end());
	m_chunksToDraw.erase(std::remove_if(m_chunksToDraw.begin(), m_chunksToDraw.end(), removeOutOfDistance), m_chunksToDraw.end());

	//for (auto it = m_chunks.cbegin(); it != m_chunks.cend(); )
	//{
	//	const Vec3I& pos = it->first;
	//	ChunkData* chunk = it->second;

	//	if ((pos.x < (chunkPos.x - HORIZONTAL_RENDER_DISTANCE)) || (pos.x > (chunkPos.x + HORIZONTAL_RENDER_DISTANCE))
	//	 || (pos.y < (chunkPos.y - VERTICAL_RENDER_DISTANCE))   || (pos.y > (chunkPos.y + VERTICAL_RENDER_DISTANCE))
	//	 || (pos.z < (chunkPos.z - HORIZONTAL_RENDER_DISTANCE)) || (pos.z > (chunkPos.z + HORIZONTAL_RENDER_DISTANCE)))
	//	{
	//		std::cout << pos << '\n';
	//		// erease returns the next item
	//		it = m_chunks.erase(it);
	//		m_freeChunks.push_back(chunk);
	//		{
	//			auto toDelete = std::find(m_chunksToGenerate.begin(), m_chunksToGenerate.end(), chunk);
	//			if (toDelete != m_chunksToGenerate.end())
	//				m_chunksToGenerate.erase(toDelete);
	//		}
	//		{
	//			auto toDelete = std::find(m_chunksToDraw.begin(), m_chunksToDraw.end(), chunk);
	//			if (toDelete != m_chunksToDraw.end())
	//				m_chunksToDraw.erase(toDelete);
	//		}
	//	}
	//	else
	//	{
	//		++it;
	//	}
	//}

	for (int32_t z = chunkPos.z - HORIZONTAL_RENDER_DISTANCE; z <= chunkPos.z + HORIZONTAL_RENDER_DISTANCE; z++)
	{
		for (int32_t y = chunkPos.y - VERTICAL_RENDER_DISTANCE; y <= chunkPos.y + VERTICAL_RENDER_DISTANCE; y++)
		{
			for (int32_t x = chunkPos.x - HORIZONTAL_RENDER_DISTANCE; x <= chunkPos.x + HORIZONTAL_RENDER_DISTANCE; x++)
			{
				const Vec3I chunkPos(x, y, z);
				auto chunk = m_chunks.find(chunkPos);
				if (chunk == m_chunks.end())
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

void ChunkSystem::initializeChunk(Chunk& chunk, const Vec3I& pos)
{
	for (size_t z = 0; z < Chunk::SIZE; z++)
	{ 
		for (size_t x = 0; x < Chunk::SIZE; x++)
		{
			double value = noise.accumulatedOctaveNoise2D_0_1((x - pos.x * 16.0) / 256.0, (z - pos.z * 16.0) / 256.0, 8) * 50;
			//double value = 0.2;
			double input = (rand() % 256) / 256.0;
			for (size_t y = 0; y < Chunk::SIZE; y++)
			{
				//double value = noise.accumulatedOctaveNoise3D_0_1((x - pos.x * 16.0) / 20.0, (z - pos.z * 16.0) / 20.0, (y - pos.y * 16.0) / 20.0, 8);
				//if (value > 0.4)
				//{
				//	chunk->operator()(x, y, z) = BlockType::Grass2;
				//}
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

//void ChunkSystem::meshChunk(const Vec3I& pos)
//{
//	ChunkStruct& chunk = chunkMesh[pos];
//
//	std::vector<GLuint>& vertices = meshFromChunk(*chunks[pos]);
//
//	chunk.vertexCount = vertices.size();
//	//vao.bind();
//	//vbo = Gfx::VertexBuffer(vertices.data(), sizeof(GLuint) * vertices.size());
//	//vbo.bind();
//	//glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)(0));
//	//glEnableVertexAttribArray(0);
//}
