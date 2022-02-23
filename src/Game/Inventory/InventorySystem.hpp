#pragma once

// Maybe move this to Player folder later and also PlayerMovementSystem and interaction system

#include <Engine/Scene.hpp>
#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Texture.hpp>
#include <Engine/Input/InputManager.hpp>
#include <Engine/Window.hpp>
#include <Game/Blocks/BlockData.hpp>
#include <Game/Inventory/Inventory.hpp>
#include <Game/Item/ItemStack.hpp>
#include <Game/Item/ItemData.hpp>
#include <Game/Item/CraftingData.hpp>
#include <Utils/Opt.hpp>

// Probably should rewrite the entire ui system. Working with normalized device coordinates is not good, but I don't know what would be better.
class InventorySystem
{
private:
	enum class Mode
	{
		Inventory,
		Crafting
	};

	struct ItemBox
	{
		Vec2 halfSize;
		Vec2 pos;
		Vec2I elementCount;
	};

public:
	InventorySystem(Scene& scene);

	[[nodiscard]] Opt<ItemStack> update(Inventory& inventory, Window& window, const InputManager& input, const ItemData& itemData);
	void render(const Inventory& inventory, const ItemData& itemData, const BlockData& blockData, const Vec2& screenSize);

	Opt<ItemStack>& heldItem(Inventory& inventory);

private:
	[[nodiscard]] Opt<ItemStack> handleInput(Inventory& inventory, const InputManager& input, const ItemData& itemData);

	void updateCrafting();
	void craftItem();

	void findSelectedSlot(Inventory& inventory, Vec2 cursorPos);
	void onItemInserted();
	void onItemTaken();

	void recalculateUi();

	ItemBox createItemBox(Vec2 pos, Vec2I elementCount);
	void drawItemBox(
		const ItemBox& itemBox,
		const Inventory& inventory,
		size_t offset,
		size_t count,
		const Texture& slotTexture,
		const BlockData& blockData,
		const ItemData& itemData);
	Vec2 getItemBoxItemPos(const ItemBox& itemBox, size_t itemIndex) const;
	Opt<size_t> getItemBoxItemIndex(const ItemBox& itemBox, Vec2 cursorPos);

	void drawItem(const ItemStack& item, Vec2 pos, const BlockData& blockData, const ItemData& itemData);

	void setupDrawItemNormal(const ItemData& blockData);
	void drawNormalItem(const ItemData::Entry& itemInfo, Vec2 pos, float size);
	void setupDrawBlockItem(const BlockData& blockData);
	void drawBlockItem(const BlockData::Entry& blockInfo, Vec2 pos, float size);

	void setupDrawText();
	void drawTextCentered(std::string_view text, Vec2 pos, float size);
	void drawTextJustifiedRight(std::string_view text, Vec2 pos, float size);

	void drawTextured(Vec2 pos, Vec2 halfSize, Vec2 textureCoordScale, const Texture& texture);
	void drawColored(Vec2 pos, Vec2 halfSize, Color color);

	Vec2 pixelScreenPosToUiScreenPos(const Vec2& pos);

	Vec2 toProportionalSize(const Vec2& vec) const;

public:
	bool isInventoryOpen;

private:
	size_t m_selectedHotbarSlot;

	Opt<ItemStack> m_heldItem;

	struct ItemBoxSlot
	{
		Opt<ItemStack>* slot;
		Vec2 pos;
		bool canPutItemsInto;
		// Set to false for crafting output for example
		bool canTakeNotAllItems;
		void (InventorySystem::*onItemTakenOrNull)();
		void (InventorySystem::* onItemInsertedOrNull)();
	};

	Opt<ItemBoxSlot> m_selectedSlot;

	Vec2 m_screenSize;

	Vec2 m_cursorPos;

	Mode m_mode;

	static constexpr float HALF_CELL_SIZE = 0.08f;
	static constexpr float CELL_SIZE = HALF_CELL_SIZE * 2;

	ItemBox m_hotbarBox;

	ItemBox m_inventoryTopBox;
	ItemBox m_inventoryBottomBox;

	Inventory m_craftingInventory;
	Inventory m_craftedItemInventory;

	ItemBox m_craftingBox;
	ItemBox m_craftedItemBox;

	Opt<CraftingData::RecipeMatch> m_craftingRecipeMatch;

	CraftingData m_craftingData;

	Vao m_squareTrianglesVao;
	Vbo m_squareTrianglesVbo;
	Vao m_cubeTrianglesVao;
	Vbo m_cubeTrianglesVbo;

	ShaderProgram m_uiTexturedShader;
	ShaderProgram m_uiColoredShader;
	ShaderProgram m_3dItemShader;
	ShaderProgram m_itemShader;
	ShaderProgram m_fontShader;

	Texture m_hotbarSlotTexture;
	Texture m_hotbarSlotSelectedTexture;
	Texture m_inventorySlotTexture;

	Texture m_inventoryTexture;

	TextureArray m_fontTextureArray;
};