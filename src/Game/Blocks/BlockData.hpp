#pragma once

#include <Game/Blocks/Block.hpp>
#include <Engine/Graphics/TextureArray.hpp>

#include <string_view>
#include <vector>

/*
	BlockData file structure

	[
		{
			textures: {
				top,
				bottom,
				left,
				right,
				front,
				back
			},
			friction,
			walkSpeed
		}
	]

*/

class BlockData
{
public:
	BlockData(const std::string_view path);

public:

	struct Entry
	{
		uint32_t topTextureIndex;
		uint32_t bottomTextureIndex;
		uint32_t leftTextureIndex;
		uint32_t rightTextureIndex;
		uint32_t frontTextureIndex;
		uint32_t backTextureIndex;
		float friction;
		float walkSpeed;
	};

	Entry& operator[](BlockType type);
	const Entry& operator[](BlockType type) const;

public:

	std::vector<Entry> blocks;

	Gfx::TextureArray textureArray;

private:
	static constexpr float DEFAULT_BLOCK_FRICTION = 0.85;

};