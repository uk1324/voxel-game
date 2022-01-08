#include <Game/Item/ItemData.hpp>

#include <unordered_map>

ItemData::ItemData()
{
	items.resize(static_cast<size_t>(ItemType::Count));
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

	auto itemEntry = [getTextureIndex](std::string_view name, std::string_view texturePath, int32_t maxItemStack) -> Entry
	{
		Entry entry;
		entry.name = name;
		entry.isBlock = false;
		entry.textureIndex = getTextureIndex(std::string(texturePath));
		entry.maxStackSize = maxItemStack;
		return entry;
	};

	auto blockEntry = [getTextureIndex](std::string_view name, BlockType blockType, int32_t maxItemStack) -> Entry
	{
		Entry entry;
		entry.name = name;
		entry.isBlock = true;
		entry.blockType = blockType;
		entry.maxStackSize = maxItemStack;
		return entry;
	};

#define ITEM_TEXTURES_PATH "assets/textures/items/"

	set(ItemType::DiamondSword, itemEntry("Diamond sword", ITEM_TEXTURES_PATH "diamond_sword.png", 1));
	set(ItemType::Stone, blockEntry("Stone", BlockType::Stone, 64));
	set(ItemType::Grass, blockEntry("Grass", BlockType::Grass, 64));
	set(ItemType::Dirt, blockEntry("Dirt", BlockType::Dirt, 64));
	set(ItemType::WoodenPlanks, blockEntry("Wodden Planks", BlockType::WoodenPlanks, 64));
	set(ItemType::Cobblestone, blockEntry("Cobblestone", BlockType::Cobblestone, 64));

#undef ITEM_TEXUTRES_PATH

	textureArray = Gfx::TextureArray(16, 16, std::vector<std::string_view>(texturesNames.begin(), texturesNames.end()));
}

ItemData::Entry& ItemData::operator[](ItemType type)
{
	return items[static_cast<size_t>(type)];
}

const ItemData::Entry& ItemData::operator[](ItemType type) const
{
	return items[static_cast<size_t>(type)];
}

ItemData::Entry& ItemData::operator[](const Item& item)
{
	return items[static_cast<size_t>(item.type)];
}

const ItemData::Entry& ItemData::operator[](const Item& item) const
{
	return items[static_cast<size_t>(item.type)];
}

void ItemData::set(ItemType type, Entry&& entry)
{
	items[static_cast<size_t>(type)] = std::forward<Entry>(entry);
}