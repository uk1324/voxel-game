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
	static constexpr size_t WIDTH = 16;
	static constexpr size_t HEIGHT = 16;
	static constexpr size_t DEPTH = 16;

private:
	Block m_data[WIDTH * HEIGHT * DEPTH];
};