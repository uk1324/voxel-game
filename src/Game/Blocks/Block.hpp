#pragma once

#include <stdint.h>

enum class BlockType : uint16_t
{
	Air = 0,
	Stone = 1,
	Grass = 2,
	Dirt = 3,
	Debug = 4,
	Count
};

struct alignas(BlockType) Block
{
public:
	Block() = default;
	Block(BlockType type);

	Block& operator=(BlockType other);

	bool operator!=(BlockType other);
	bool operator==(BlockType other);

	bool isSolid();


public:
	static constexpr int SIZE = 1;

public:
	BlockType type;
};