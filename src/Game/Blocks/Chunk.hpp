#pragma once

#include <Game/Blocks/Block.hpp>

class Chunk
{
public:
	static constexpr size_t WIDTH = 16;
	static constexpr size_t HEIGHT = 16;
	static constexpr size_t DEPTH = 16;

	Block get(size_t x, size_t y, size_t z);

private:
	Block data[WIDTH * HEIGHT * DEPTH];
};