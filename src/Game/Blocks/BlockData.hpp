#pragma once

#include <Game/Blocks/Block.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Game/Item/ItemStack.hpp>

#include <string_view>
#include <vector>

// TODO: Make this global or static.
class BlockData
{
public:
	struct Entry
	{
		Entry() : drop(ItemType::Dirt, 0) {};

		union
		{
			struct
			{
				uint32_t topTextureIndex;
				uint32_t bottomTextureIndex;
				uint32_t leftTextureIndex;
				uint32_t rightTextureIndex;
				uint32_t frontTextureIndex;
				uint32_t backTextureIndex;
			};
			uint32_t textureIndex;

			struct
			{
				float friction;
			} liquid;
		};
		// TODO Maybe make an enum BlockType with solid decoration and liquid. Non 
		float friction;
		float walkSpeed;
		bool isSolid;
		bool isDecoration;
		bool isLiquid;
		ItemStack drop;
	};

public:
	BlockData();

	Entry& operator[](BlockType type);
	const Entry& operator[](BlockType type) const;

	void set(BlockType block, Entry&& entry);

public:

	std::vector<Entry> blocks;

	TextureArray textureArray;

private:
	static constexpr float DEFAULT_BLOCK_FRICTION = 0.85f;

};