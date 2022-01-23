#include <Game/Blocks/Structure.hpp>

StructureData::StructureData(const Vec3I& size)
	: size(size)
	, blocks(static_cast<size_t>(size.x) * static_cast<size_t>(size.y) * static_cast<size_t>(size.z))
{}

void StructureData::set(Vec3I pos, BlockType block)
{
	blocks.data()[pos.z * size.x * size.y + pos.y * size.x + pos.x] = block;
}

BlockType StructureData::get(Vec3I pos) const
{
	return blocks.data()[pos.z * size.x * size.y + pos.y * size.x + pos.x].type;
}

Structure::Structure(const Vec3I& pos, const StructureData& data)
	: pos(pos)
	, data(&data)
{
	std::is_trivially_copyable_v<Structure>;
}
