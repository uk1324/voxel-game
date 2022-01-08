#include <Game/Blocks/BlockData.hpp>
#include <Utils/FileIo.hpp>
#include <Utils/Assertions.hpp>

BlockData::BlockData(const std::string_view path)
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
	
	blocks.resize(static_cast<size_t>(BlockType::Count));
	for (size_t i = 0; i < static_cast<size_t>(BlockType::Count); i++)
	{
		blocks[i] = {
			getTextureIndex("assets/textures/blocks/dirt.png"),
			getTextureIndex("assets/textures/blocks/dirt.png"),
			getTextureIndex("assets/textures/blocks/dirt.png"),
			getTextureIndex("assets/textures/blocks/dirt.png"),
			getTextureIndex("assets/textures/blocks/dirt.png"),
			getTextureIndex("assets/textures/blocks/dirt.png"),
			0.85,
			1,
		};
	}

	blocks[static_cast<size_t>(BlockType::Stone) - 1] = {
		getTextureIndex("assets/textures/blocks/stone.png"),
		getTextureIndex("assets/textures/blocks/stone.png"),
		getTextureIndex("assets/textures/blocks/stone.png"),
		getTextureIndex("assets/textures/blocks/stone.png"),
		getTextureIndex("assets/textures/blocks/stone.png"),
		getTextureIndex("assets/textures/blocks/stone.png"),
		0.85,
		1,
	};

	blocks[static_cast<size_t>(BlockType::Grass) - 1] = {
		getTextureIndex("assets/textures/blocks/grass.png"),
		getTextureIndex("assets/textures/blocks/dirt.png"),
		getTextureIndex("assets/textures/blocks/grass_side.png"),
		getTextureIndex("assets/textures/blocks/grass_side.png"),
		getTextureIndex("assets/textures/blocks/grass_side.png"),
		getTextureIndex("assets/textures/blocks/grass_side.png"),
		0.85,
		1,
	};

	blocks[static_cast<size_t>(BlockType::WoodenPlanks) - 1] = {
		getTextureIndex("assets/textures/blocks/wooden_planks.png"),
		getTextureIndex("assets/textures/blocks/wooden_planks.png"),
		getTextureIndex("assets/textures/blocks/wooden_planks.png"),
		getTextureIndex("assets/textures/blocks/wooden_planks.png"),
		getTextureIndex("assets/textures/blocks/wooden_planks.png"),
		getTextureIndex("assets/textures/blocks/wooden_planks.png"),
		0.85,
		1,
	};

	blocks[static_cast<size_t>(BlockType::Cobblestone) - 1] = {
		getTextureIndex("assets/textures/blocks/cobblestone.png"),
		getTextureIndex("assets/textures/blocks/cobblestone.png"),
		getTextureIndex("assets/textures/blocks/cobblestone.png"),
		getTextureIndex("assets/textures/blocks/cobblestone.png"),
		getTextureIndex("assets/textures/blocks/cobblestone.png"),
		getTextureIndex("assets/textures/blocks/cobblestone.png"),
		0.85,
		1,
	};

	blocks[static_cast<size_t>(BlockType::Debug) - 1] = {
		getTextureIndex("assets/textures/blocks/top.png"),
		getTextureIndex("assets/textures/blocks/bottom.png"),
		getTextureIndex("assets/textures/blocks/left.png"),
		getTextureIndex("assets/textures/blocks/right.png"),
		getTextureIndex("assets/textures/blocks/front.png"),
		getTextureIndex("assets/textures/blocks/back.png"),
		0.85,
		1,
	};

	textureArray = Gfx::TextureArray(16, 16, std::vector<std::string_view>(texturesNames.begin(), texturesNames.end()));
}

BlockData::Entry& BlockData::operator[](BlockType type)
{
	ASSERT(type != BlockType::Air);
	return blocks[static_cast<size_t>(type) - 1];
}

const BlockData::Entry& BlockData::operator[](BlockType type) const
{
	ASSERT(type != BlockType::Air);
	return blocks[static_cast<size_t>(type) - 1];;
}
