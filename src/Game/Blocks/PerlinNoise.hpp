#pragma once

#include <Math/Vec3.hpp>

#include <random>

class PerlinNoise
{
public:
	PerlinNoise(uint64_t seed);
	
	float at(const Vec3& p) const;
	float accumulatedOctaveAt(const Vec3& p) const;

private:
	int hash(int x, int y, int z) const;

private:
	static constexpr size_t GRADIENT_TABLE_SIZE = 256;

private:
	// Not using std::array because the bounds checks make it way to slow in debug mode.
	Vec3 m_gradients[GRADIENT_TABLE_SIZE];
	// Permutation table size is twice as big the gradient table because the permutation hash function returns values in range 0 to 511.z
	uint8_t m_permutations[GRADIENT_TABLE_SIZE * 2];
};