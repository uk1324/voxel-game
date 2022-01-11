#include <Game/Blocks/Chunk.hpp>

Chunk::Chunk()
{
	for (size_t i = 0; i < (SIZE * SIZE * SIZE); i++)
		m_data[i] = BlockType::Air;
}

Block& Chunk::operator()(size_t x, size_t y, size_t z)
{
	return m_data[z * (SIZE_SQUARED) + y * SIZE + x];
}

Block Chunk::operator()(size_t x, size_t y, size_t z) const
{
	return m_data[z * (SIZE_SQUARED) + y * SIZE + x];
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
