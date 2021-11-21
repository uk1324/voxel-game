#include <Game/Blocks/Block.hpp>

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
