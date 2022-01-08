#pragma once

#include <Game/Item/Item.hpp>
#include <Game/Blocks/Block.hpp>
#include <Engine/Graphics/TextureArray.hpp>

#include <string>
#include <string_view>
#include <vector>

class ItemData
{
public:
	struct Entry
	{
		std::string name;
		union
		{
			uint32_t textureIndex;
			BlockType blockType;
		};
		bool isBlock;
		int32_t maxStackSize;
	};

public:
	ItemData();

	Entry& operator[](ItemType type);
	const Entry& operator[](ItemType type) const;
	Entry& operator[](const Item& item);
	const Entry& operator[](const Item& item) const;

	void set(ItemType type, Entry&& entry);

public:
	std::vector<Entry> items;

	Gfx::TextureArray textureArray;
};