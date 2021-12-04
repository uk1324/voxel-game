#include <Game/Blocks/ChunkSystem.hpp>

#include <glad/glad.h>

ChunkSystem::ChunkSystem()
// lastPos is infinity so all the chunks get generated at the start
	: lastPos(Vec3I(std::numeric_limits<float>::infinity(), 0, 0))
	, noise()
{
	static constexpr int CHUNKS_IN_RENDER_DISTANCE = VERTICAL_RENDER_DISTANCE * HORIZONTAL_RENDER_DISTANCE * HORIZONTAL_RENDER_DISTANCE;
	chunkPool.resize(CHUNKS_IN_RENDER_DISTANCE);
	for (int i = 0; i < CHUNKS_IN_RENDER_DISTANCE; i++)
	{
		chunkPool.push_back(std::make_unique<Chunk>());
	}

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

static std::vector<GLuint>& meshFromChunk(Chunk& chunk)
{
	static std::vector<GLuint> vertices(16 * 16 * 16 * 6 * 3);
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

void ChunkSystem::update(const Vec3& p)
{
	if (chunksToGenerate.size() != 0)
	{
		const Vec3I& chunkPos = chunksToGenerate.back();
		initializeChunk(chunks[chunkPos], chunkPos);
		meshChunk(chunkPos);
		chunksToGenerate.pop_back();
	}

	Vec3I position(p.x / Chunk::SIZE, p.y / Chunk::SIZE, p.z / Chunk::SIZE);
	if (position == lastPos)
	{
		lastPos = position;
		deletedChunks.clear();
		return; 
	}
	lastPos = position;

	deletedChunks.clear();
	for (const auto& [pos, chunk] : chunks)
	{
		if (pos.x < position.x - HORIZONTAL_RENDER_DISTANCE || pos.x > position.x + HORIZONTAL_RENDER_DISTANCE
		 || pos.y < position.y - VERTICAL_RENDER_DISTANCE || pos.y > position.y + VERTICAL_RENDER_DISTANCE
		 || pos.z < position.z - HORIZONTAL_RENDER_DISTANCE || pos.z > position.z + HORIZONTAL_RENDER_DISTANCE)
		{
			deletedChunks.push_back(pos);
			auto toDelete = std::find(chunksToGenerate.begin(), chunksToGenerate.end(), pos);
			if (toDelete != chunksToGenerate.end())
			{
				chunksToGenerate.erase(toDelete);
			}

			auto toDeleteMesh = std::find(chunkToMesh.begin(), chunkToMesh.end(), pos);
			if (toDeleteMesh != chunkToMesh.end())
			{
				chunkToMesh.erase(toDeleteMesh);
			}
		}
	}

	for (const auto& chunkPos : deletedChunks)
	{
		freeChunks.push_back(chunks[chunkPos]);
		chunks.erase(chunkPos);
	}

	for (int y = position.y - VERTICAL_RENDER_DISTANCE; y <= position.y + VERTICAL_RENDER_DISTANCE; y++)
	{
		for (int z = position.z - HORIZONTAL_RENDER_DISTANCE; z <= position.z + HORIZONTAL_RENDER_DISTANCE; z++)
		{
			for (int x = position.x - HORIZONTAL_RENDER_DISTANCE; x <= position.x + HORIZONTAL_RENDER_DISTANCE; x++)
			{
				const auto& chunk = chunks.find(Vec3I(x, y, z));
				if (chunk == chunks.end())
				{
					Chunk* chunk;
					if (freeChunks.size() == 0)
					{
						chunk = new Chunk;
						chunkPool.push_back(std::unique_ptr<Chunk>(chunk));
						chunks[Vec3I(x, y, z)] = chunk;
					}
					else
					{
						chunk = freeChunks.back();
						freeChunks.pop_back();
					}
					chunks[Vec3I(x, y, z)] = chunk;
					chunksToGenerate.push_back(Vec3I(x, y, z));
				}
			}
		}
	}

	std::sort(chunksToGenerate.begin(), chunksToGenerate.end(),
		[&position](const Vec3I& a, const Vec3I& b) {
			return Vec3I::distanceSquared(a, position) > Vec3I::distanceSquared(b, position);
		}
	); 
}

void ChunkSystem::initializeChunk(Chunk* chunk, const Vec3I& pos)
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
						chunk->operator()(x, y, z) = BlockType::Grass2;
					}
					else if (realY < 30 && realY > 25)
					{
						if (rand() % 2 == 0)
						{
							chunk->operator()(x, y, z) = BlockType::Grass2;
						}
						else
						{
							chunk->operator()(x, y, z) = BlockType::Grass;
						}
					}
					else
					{
						chunk->operator()(x, y, z) = BlockType::Grass;
					}
				}
				else
				{
					chunk->operator()(x, y, z) = BlockType::Air;
				}
			}
		}
	}
}

void ChunkSystem::meshChunk(const Vec3I& pos)
{
	chunkMesh[pos] = ChunkStruct();
	ChunkStruct& chunk = chunkMesh[pos];

	std::vector<GLuint>& vertices = meshFromChunk(*chunks[pos]);

	chunk.vertexCount = vertices.size();
	chunk.vao.bind();
	chunk.vbo = Gfx::VertexBuffer(vertices.data(), sizeof(GLuint) * vertices.size());
	chunk.vbo.bind();
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(GLuint), (void*)(0));
	glEnableVertexAttribArray(0);
}
