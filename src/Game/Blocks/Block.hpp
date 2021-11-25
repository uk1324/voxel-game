#pragma once

#include <stdint.h>

enum class BlockType : uint16_t
{
	Air = 0,
	Dirt = 1,
	Stone = 2,
	Cobblestone = 3
};

struct alignas(BlockType) Block
{
public:
	Block& operator=(BlockType other);

	bool operator!=(BlockType other);
	bool operator==(BlockType other);

public:
	BlockType type;
};