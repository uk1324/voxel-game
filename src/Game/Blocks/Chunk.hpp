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
	static constexpr int32_t SIZE_X = 15;
	static constexpr int32_t SIZE_Z = 15;
	static constexpr int32_t SIZE_Y = 127;
	static constexpr int32_t BLOCK_COUNT = SIZE_X * SIZE_Z * SIZE_Y;
	static const Vec3 SIZE_V;
	static const Vec3I SIZE_INT;

private:
	Block m_data[BLOCK_COUNT];
};