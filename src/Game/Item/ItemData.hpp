#pragma once

#include <Game/Item/Item.hpp>
#include <Game/Blocks/Block.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/Vbo.hpp>
#include <Math/Vec3.hpp>
#include <Image/Image32.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

class ItemData
{
public:
	struct VoxelizedItemModel
	{
		size_t vboVertexOffset;
		size_t vertexCount;
	};

	struct Entry
	{
		std::string name;
		union
		{
			struct
			{
				uint32_t textureIndex;
				VoxelizedItemModel model;
			};
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

	TextureArray textureArray;

	struct VoxelizedItemModelVertex
	{
		Vec3 pos;
		Vec3 color;
	};
	std::unordered_map<ItemType, VoxelizedItemModel> voxelizedItemModels;

	Vao voxelizedItemModelsVao;
	Vbo voxelizedItemModelsVbo;

private:
	VoxelizedItemModel generateVoxelizedItemModel(std::vector<VoxelizedItemModelVertex>& vertices, const Image32& image);
};