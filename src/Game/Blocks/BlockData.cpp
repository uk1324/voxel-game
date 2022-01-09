#include <Game/Blocks/BlockData.hpp>
#include <Utils/FileIo.hpp>
#include <Utils/Assertions.hpp>

BlockData::BlockData()
{
	std::vector<std::string> texturesNames;
	std::unordered_map<std::string, uint32_t> textureNameToIndex;

	auto getTextureIndex = [&texturesNames, &textureNameToIndex](const std::string& name) -> uint32_t
	{
		auto textureIndex = textureNameToIndex.find(name);
		if (textureIndex == textureNameToIndex.end())
		{
			texturesNames.push_back(name);
			size_t textureIndex = texturesNames.size() - 1;
			textureNameToIndex[name] = textureIndex;
			return textureIndex;
		}
		return textureIndex->second;
	};
	
	auto entry = [getTextureIndex]
	(
		std::string top,
		std::string bottom,
		std::string left,
		std::string right,
		std::string front,
		std::string back,
		float friction,
		float walkSpeed,
		bool isSolid
	)
	{
		Entry entry;
		entry.topTextureIndex = getTextureIndex(top);
		entry.bottomTextureIndex = getTextureIndex(bottom);
		entry.leftTextureIndex = getTextureIndex(left);
		entry.rightTextureIndex = getTextureIndex(right);
		entry.frontTextureIndex = getTextureIndex(front);
		entry.backTextureIndex = getTextureIndex(back);
		entry.friction = friction;
		entry.walkSpeed = walkSpeed;
		entry.isSolid = isSolid;
		return entry;
	};

	blocks.resize(static_cast<size_t>(BlockType::Count));

#define PATH "assets/textures/blocks/"

	set(BlockType::Stone, entry(
		PATH "stone.png",
		PATH "stone.png",
		PATH "stone.png",
		PATH "stone.png",
		PATH "stone.png",
		PATH "stone.png",
		0.85,
		1,
		true
	));

	set(BlockType::Grass, entry(
		PATH "grass.png",
		PATH "dirt.png",
		PATH "grass_side.png",
		PATH "grass_side.png",
		PATH "grass_side.png",
		PATH "grass_side.png",
		0.85,
		1,
		true
	));

	set(BlockType::Dirt, entry(
		PATH "dirt.png",
		PATH "dirt.png",
		PATH "dirt.png",
		PATH "dirt.png",
		PATH "dirt.png",
		PATH "dirt.png",
		0.85,
		1,
		true
	));

	set(BlockType::WoodenPlanks, entry(
		PATH "wooden_planks.png",
		PATH "wooden_planks.png",
		PATH "wooden_planks.png",
		PATH "wooden_planks.png",
		PATH "wooden_planks.png",
		PATH "wooden_planks.png",
		0.85,
		1,
		true
	));

	set(BlockType::Cobblestone, entry(
		PATH "cobblestone.png",
		PATH "cobblestone.png",
		PATH "cobblestone.png",
		PATH "cobblestone.png",
		PATH "cobblestone.png",
		PATH "cobblestone.png",
		0.85,
		1,
		true
	));

	set(BlockType::Debug, entry(
		PATH "top.png",
		PATH "bottom.png",
		PATH "left.png",
		PATH "right.png",
		PATH "front.png",
		PATH "back.png",
		0.85,
		1,
		false
	));

	textureArray = Gfx::TextureArray(16, 16, std::vector<std::string_view>(texturesNames.begin(), texturesNames.end()));
}

// Functions subtract 1 to skip BlockType::Air.
BlockData::Entry& BlockData::operator[](BlockType type)
{
	ASSERT(type != BlockType::Air);
	return blocks[static_cast<size_t>(type) - 1];
}

const BlockData::Entry& BlockData::operator[](BlockType type) const
{
	ASSERT(type != BlockType::Air);
	return blocks[static_cast<size_t>(type) - 1];
}

void BlockData::set(BlockType block, Entry&& entry)
{
	blocks[static_cast<size_t>(block) - 1] = std::forward<Entry>(entry);
}
