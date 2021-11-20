#pragma once

#include <stdint.h>

enum class BlockType : uint16_t
{
	Dirt
};

struct alignas(BlockType) Block
{
public:
	BlockType type;
};