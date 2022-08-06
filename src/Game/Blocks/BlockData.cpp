#include <Game/Blocks/BlockData.hpp>
#include <Utils/FileIo.hpp>
#include <Utils/Assertions.hpp>

BlockData::BlockData()
	: textureArray(TextureArray::null())
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
		bool isSolid,
		ItemStack drop
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
		entry.isDecoration = false;
		entry.isLiquid = false;
		entry.drop = drop;
		return entry;
	};

	auto decorationEntry = [getTextureIndex]
	(
		std::string texture
	)
	{
		Entry entry;
		entry.textureIndex = getTextureIndex(texture);
		entry.isSolid = false;
		entry.isDecoration = true;
		entry.isLiquid = false;
		entry.drop = ItemStack(Item(ItemType::Dirt), 1);
		return entry;
	};

	auto liquidEntry = [getTextureIndex]
	(
		std::string texture,
		float liquidFriction,
		ItemStack drop
	)
	{
		Entry entry;
		entry.topTextureIndex = getTextureIndex(texture);
		entry.bottomTextureIndex = getTextureIndex(texture);
		entry.leftTextureIndex = getTextureIndex(texture);
		entry.rightTextureIndex = getTextureIndex(texture);
		entry.frontTextureIndex = getTextureIndex(texture);
		entry.backTextureIndex = getTextureIndex(texture);
		entry.isSolid = false;
		entry.isDecoration = false;
		entry.isLiquid = true;
		entry.liquid.friction = liquidFriction;
		entry.friction = 0.85f;
		entry.walkSpeed = 1.0f;
		entry.drop = drop;
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
		true,
		ItemStack(ItemType::Stone, 1)
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
		true,
		ItemStack(ItemType::Grass, 1)
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
		true,
		ItemStack(ItemType::Dirt, 1)
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
		true,
		ItemStack(ItemType::WoodenPlanks, 1)
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
		true,
		ItemStack(ItemType::Cobblestone, 1)
	));

	set(BlockType::Sand, entry(
		PATH "sand.png",
		PATH "sand.png",
		PATH "sand.png",
		PATH "sand.png",
		PATH "sand.png",
		PATH "sand.png",
		0.85,
		1,
		true,
		ItemStack(ItemType::Dirt, 1)
	));

	set(BlockType::Leaves, entry(
		PATH "leaves.png",
		PATH "leaves.png",
		PATH "leaves.png",
		PATH "leaves.png",
		PATH "leaves.png",
		PATH "leaves.png",
		0.85,
		1,
		true,
		ItemStack(ItemType::Dirt, 1)
	));


	set(BlockType::Log, entry(
		PATH "log_top_bottom.png",
		PATH "log_top_bottom.png",
		PATH "log_side.png",
		PATH "log_side.png",
		PATH "log_side.png",
		PATH "log_side.png",
		0.85,
		1,
		true,
		ItemStack(ItemType::Dirt, 1)
	));

	set(BlockType::Flower, decorationEntry(PATH "flower.png"));

	set(BlockType::Debug, entry(
		PATH "top.png",
		PATH "bottom.png",
		PATH "left.png",
		PATH "right.png",
		PATH "front.png",
		PATH "back.png",
		0.85,
		1,
		false,
		ItemStack(ItemType::Debug, 1)
	));

	set(BlockType::Water, liquidEntry(PATH "water.png", 0.85f, ItemStack(Item(ItemType::Debug), 1)));

	textureArray = TextureArray(16, 16, std::vector<std::string_view>(texturesNames.begin(), texturesNames.end()));
}

// Maybe having a blockType for air with some random data would allow for some code like for example meshing 
// to have less branches though it would be more error prone.
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
