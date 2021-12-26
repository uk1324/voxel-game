#pragma once

#include <Game/Blocks/Block.hpp>

// TODO: Find what order of axis is good for iteration

class Chunk
{
public:
	Chunk();

	Block& operator() (size_t x, size_t y, size_t z);
	Block operator() (size_t x, size_t y, size_t z) const;
	Block get(size_t x, size_t y, size_t z) const;
	Block set(size_t x, size_t y, size_t z, Block value);

public:
	static constexpr uint32_t SIZE = 16;
	static constexpr uint32_t SIZE_SQUARED = SIZE * SIZE;
	static constexpr uint32_t SIZE_CUBED = SIZE * SIZE * SIZE;

private:
	Block m_data[SIZE_CUBED];
};