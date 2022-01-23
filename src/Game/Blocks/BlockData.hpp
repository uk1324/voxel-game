#pragma once

#include <Game/Blocks/Block.hpp>
#include <Engine/Graphics/TextureArray.hpp>

#include <string_view>
#include <vector>

class BlockData
{
public:
	struct Entry
	{
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
		};
		float friction;
		float walkSpeed;
		bool isSolid;
		bool isDecoration;
	};

public:
	BlockData();

	Entry& operator[](BlockType type);
	const Entry& operator[](BlockType type) const;

	void set(BlockType block, Entry&& entry);

public:

	std::vector<Entry> blocks;

	Gfx::TextureArray textureArray;

private:
	static constexpr float DEFAULT_BLOCK_FRICTION = 0.85f;

};