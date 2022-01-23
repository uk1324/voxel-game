#pragma once

#include <Math/Vec3.hpp>
#include <Game/Blocks/Block.hpp>

#include <vector>

class StructureData
{
public:
	StructureData(const Vec3I& size);

	void set(Vec3I pos, BlockType block);
	BlockType get(Vec3I pos) const;

public:
	Vec3I size;
	std::vector<Block> blocks;
};

#include <type_traits>

class Structure
{
public:
	Structure(const Vec3I& pos, const StructureData& data);

public:
	Vec3I pos;
	const StructureData* data;
};