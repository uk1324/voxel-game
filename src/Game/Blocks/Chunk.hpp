#pragma once

#include <Game/Blocks/Block.hpp>
#include <Math/Vec3.hpp>

// TODO: Find what order of axis is good for iteration

class Chunk
{
public:
	Chunk();

	Block& operator() (size_t x, size_t y, size_t z);
	Block operator() (size_t x, size_t y, size_t z) const;
	Block get(size_t x, size_t y, size_t z) const;
	Block get(const Vec3I& pos) const;
	void set(size_t x, size_t y, size_t z, Block value);
	void set(const Vec3I& pos, Block value);

public:
	static constexpr int32_t SIZE = 16;
	static constexpr int32_t SIZE_SQUARED = SIZE * SIZE;
	static constexpr int32_t SIZE_CUBED = SIZE * SIZE * SIZE;

private:
	Block m_data[SIZE_CUBED];
};