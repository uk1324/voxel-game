#include <Game/Item/ItemData.hpp>
#include <Math/Vec2.hpp>

#include <unordered_map>

#include <iostream>

ItemData::ItemData()
	: textureArray(TextureArray::null())
{
	items.resize(static_cast<size_t>(ItemType::Count));
	std::vector<std::string> texturesNames;
	std::vector<ItemType> textureItemType;
	std::unordered_map<std::string, uint32_t> textureNameToIndex;

	std::vector<VoxelizedItemModelVertex> voxelizedItemsVertices;

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

	auto itemEntry = [getTextureIndex, &voxelizedItemsVertices, this](std::string_view name, std::string_view texturePath, int32_t maxItemStack) -> Entry
	{
		Entry entry;
		entry.name = name;
		entry.isBlock = false;
		entry.textureIndex = getTextureIndex(std::string(texturePath));
		entry.maxStackSize = maxItemStack;
		Image32 image(texturePath.data());
		entry.model = generateVoxelizedItemModel(voxelizedItemsVertices,  image);
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

#define PATH "assets/textures/items/"

	set(ItemType::DiamondSword, itemEntry("Diamond sword", PATH "diamond_sword.png", 1));
	set(ItemType::Stone, blockEntry("Stone", BlockType::Stone, 64));
	set(ItemType::Grass, blockEntry("Grass", BlockType::Grass, 64));
	set(ItemType::Dirt, blockEntry("Dirt", BlockType::Dirt, 64));
	set(ItemType::WoodenPlanks, blockEntry("Wodden Planks", BlockType::WoodenPlanks, 64));
	set(ItemType::Cobblestone, blockEntry("Cobblestone", BlockType::Cobblestone, 64));
	set(ItemType::Debug, blockEntry("Debug", BlockType::Debug, 64));
	set(ItemType::Water, blockEntry("Water", BlockType::Water, 64));

#undef PATH

	voxelizedItemModelsVao.bind();
	voxelizedItemModelsVbo = Vbo(voxelizedItemsVertices.data(), voxelizedItemsVertices.size() * sizeof(VoxelizedItemModelVertex));
	voxelizedItemModelsVbo.bind();
	Vao::setAttribute(0, ShaderDataType::Float, 3, false, sizeof(VoxelizedItemModelVertex), offsetof(VoxelizedItemModelVertex, pos));
	Vao::setAttribute(1, ShaderDataType::Float, 3, false, sizeof(VoxelizedItemModelVertex), offsetof(VoxelizedItemModelVertex, color));
	Vao::setAttribute(2, ShaderDataType::Float, 3, false, sizeof(VoxelizedItemModelVertex), offsetof(VoxelizedItemModelVertex, normal));
	textureArray = TextureArray(16, 16, std::vector<std::string_view>(texturesNames.begin(), texturesNames.end()));
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

ItemData::VoxelizedItemModel ItemData::generateVoxelizedItemModel(std::vector<VoxelizedItemModelVertex>& vertices, const Image32& image)
{
	size_t vertexCountStart = vertices.size();

	for (size_t y = 0; y < image.height(); y++)
	{
		for (size_t x = 0; x < image.width(); x++)
		{
			const auto color32 = image.get(x, y);
			if (color32.a != 255)
				continue;

			const Vec3 color(color32.r / 255.0f, color32.g / 255.0f, color32.b / 255.0f);

			auto add = [&vertices, &color, &image](float x, float y, float z, const Vec3& normal) -> void
			{
				Vec3 pos(x - image.width() / 2.0f, y - image.height() / 2.0f, z);
				pos.x /= image.width();
				pos.y /= image.width();
				pos.z /= 16.0f;

				vertices.push_back(VoxelizedItemModelVertex{ pos, color, normal });
			};

			// Could just compute the offsets.
			auto addFront = [&add](float x, float y) -> void
			{
				const auto& n = Vec3::forward;
				add(x, y, 0.5f, n);
				add(x + 1.0f, y + 1.0f, 0.5f, n);
				add(x + 1.0f, y, 0.5f, n);

				add(x, y, 0.5f, n);
				add(x, y + 1.0f, 0.5f, n);
				add(x + 1.0f, y + 1.0f, 0.5f, n);
			};

			auto addBack = [&add](float x, float y) -> void
			{
				const auto& n = Vec3::back;
				add(x, y, -0.5f, n);
				add(x + 1.0f, y, -0.5f, n);
				add(x + 1.0f, y + 1.0f, -0.5f, n);

				add(x, y, -0.5f, n);
				add(x + 1.0f, y + 1.0f, -0.5f, n);
				add(x, y + 1.0f, -0.5f, n);
			};

			auto addLeft = [&add](float x, float y) -> void
			{
				const auto& n = Vec3::left;
				add(x, y, -0.5f, n);
				add(x, y + 1.0f, 0.5f, n);
				add(x, y, 0.5f, n);

				add(x, y, -0.5f, n);
				add(x, y + 1.0f, -0.5f, n);
				add(x, y + 1.0f, 0.5f, n);
			};

			auto addRight = [&add](float x, float y) -> void
			{
				const auto& n = Vec3::right;
				add(x + 1.0f, y, -0.5f, n);
				add(x + 1.0f, y, 0.5f, n);
				add(x + 1.0f, y + 1.0f, 0.5f, n);

				add(x + 1.0f, y, -0.5f, n);
				add(x + 1.0f, y + 1.0f, 0.5f, n);
				add(x + 1.0f, y + 1.0f, -0.5f, n);
			};

			auto addBottom = [&add](float x, float y) -> void
			{
				const auto& n = Vec3::down;
				add(x, y, -0.5f, n);
				add(x, y, 0.5f, n);
				add(x + 1.0f, y, 0.5f, n);

				add(x, y, -0.5f, n);
				add(x + 1.0f, y, 0.5f, n);
				add(x + 1.0f, y, -0.5f, n);
			};

			auto addTop = [&add](float x, float y) -> void
			{
				const auto& n = Vec3::up;
				add(x, y + 1.0f, -0.5f, n);
				add(x + 1.0f, y + 1.0f, 0.5f, n);
				add(x, y + 1.0f, 0.5f, n);

				add(x, y + 1.0f, -0.5f, n);
				add(x + 1.0f, y + 1.0f, -0.5f, n);
				add(x + 1.0f, y + 1.0f, 0.5f, n);
			};

			addFront(x, y);
			addBack(x, y);
			if ((x == 0) || (image.get(x - 1, y).a != 255))
			{
				addLeft(x, y);
			}
			if ((x == (image.width() - 1)) || (image.get(x + 1, y).a != 255))
			{
				addRight(x, y);
			}
			if ((y == 0) || (image.get(x, y - 1).a != 255))
			{
				addBottom(x, y);
			}
			if ((y == (image.height() - 1)) || (image.get(x, y + 1).a != 255))
			{
				addTop(x, y);
			}
		}
	}

	return VoxelizedItemModel{ vertexCountStart, vertices.size() -  vertexCountStart };
}