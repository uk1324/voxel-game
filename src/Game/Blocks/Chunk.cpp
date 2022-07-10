#include <Game/Blocks/Chunk.hpp>

Chunk::Chunk()
{
	for (size_t i = 0; i < BLOCK_COUNT; i++)
		m_data[i] = BlockType::Air;
}

Block& Chunk::operator()(size_t x, size_t y, size_t z)
{
	return m_data[z * (SIZE_Y * SIZE_X) + y * SIZE_X + x];
	//return m_data[z * (SIZE_Y * SIZE_X) + x * SIZE_Y + y];
}

Block Chunk::operator()(size_t x, size_t y, size_t z) const
{
	return m_data[z * (SIZE_Y * SIZE_X) + y * SIZE_X + x];
	//return m_data[z * (SIZE_Y * SIZE_X) + x * SIZE_Y + y];
}

Block Chunk::get(size_t x, size_t y, size_t z) const
{
	return operator()(x, y, z);
}

Block Chunk::get(const Vec3I& pos) const
{
	return operator()(pos.x, pos.y, pos.z);
}

void Chunk::set(size_t x, size_t y, size_t z, Block value)
{
	operator()(x, y, z) = value;
}

void Chunk::set(const Vec3I& pos, Block value)
{
	operator()(pos.x, pos.y, pos.z) = value;
}

const Vec3 Chunk::SIZE_V = Vec3(SIZE_X, SIZE_Y, SIZE_Z);
const Vec3I Chunk::SIZE_INT = Vec3I(SIZE_X, SIZE_Y, SIZE_Z);