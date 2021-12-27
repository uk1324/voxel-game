#include "Block.hpp"
#include "Block.hpp"
#include "Block.hpp"
#include <Game/Blocks/Block.hpp>

Block::Block(BlockType type)
	: type(type)
{}

Block& Block::operator= (BlockType other)
{
	type = other;
	return *this;
}

bool Block::operator!= (BlockType other)
{
	return type != other;
}

bool Block::operator== (BlockType other)
{
	return type == other;
}

bool Block::isSolid()
{
	return type != BlockType::Air;
}
