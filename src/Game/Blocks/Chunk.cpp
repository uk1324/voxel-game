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

Block Chunk::set(size_t x, size_t y, size_t z, Block value)
{
	return operator()(x, y, z) = value;
}
