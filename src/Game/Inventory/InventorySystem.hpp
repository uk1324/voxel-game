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
#include <Utils/Opt.hpp>

// Probably should rewrite the entire ui system. Working with normalized device coordinates is not good, but I don't know what would be better.


class InventorySystem
{
private:
	struct UiElement
	{
		Vec2 size;
		Vec2 pos;
	};

public:
	InventorySystem(Scene& scene);

	[[nodiscard]] Opt<ItemStack> update(Inventory& inventory, Window& window, const InputManager& input, const ItemData& itemData);
	void render(const Inventory& inventory, const ItemData& itemData, const BlockData& blockData, const Vec2& screenSize);

	Opt<ItemStack>& heldItem(Inventory& inventory);

private:
	[[nodiscard]] Opt<ItemStack> handleInput(Inventory& inventory, const InputManager& input, const ItemData& itemData);

	void drawHotbar(const Inventory& inventory, const BlockData& blockData, const ItemData& itemData);
	void drawInventory(const Inventory& inventory, const BlockData& blockData, const ItemData& itemData);

	void setupDrawItemNormal(const ItemData& blockData);
	void drawNormalItem(const ItemData::Entry& itemInfo, Vec2 pos, float size);
	void setupDrawBlockItem(const BlockData& blockData);
	void drawBlockItem(const BlockData::Entry& blockInfo, Vec2 pos, float size);

	void setupDrawText();
	void drawTextCentered(std::string_view text, Vec2 pos, float size);
	void drawTextJustifiedRight(std::string_view text, Vec2 pos, float size);

	void drawUiElementTextured(const UiElement& element, const Vec2& textureCoordScale);

	Opt<size_t> getInventoryItemAt(Vec2 screenPos);

	Vec2 getHotbarSlotPos(size_t pos);
	Vec2 getInventorySlotPos(size_t pos);

	void recalculateUi();

	float yPercentToXPercent(float yPercent);

	Vec2 pixelScreenPosToUiScreenPos(const Vec2& pos);

private:
	static bool contains(const UiElement& element, const Vec2& point);

public:
	bool isInventoryOpen;

private:

	size_t m_selectedHotbarSlot;
	Opt<size_t> m_selectedInventorySlot;
	Opt<ItemStack> m_heldItem;


	Vec2 m_screenSize;

	Vec2 m_cursorPos;

	static constexpr float SCREEN_BOTTOM = -1.0f;

	static constexpr float INVENTORY_X_ELEMENT_COUNT = 9.0f;
	static constexpr float INVENTORY_Y_ELEMENT_COUNT = 3.0f;
	static constexpr float INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT = 0.1f;
	static constexpr float DISTANCE_BETWEEN_INVENTORY_TOP_AND_INVENTORY_BOTTOM_Y_PERCENT = 0.1f;
	static constexpr float ITEM_NORMAL_SCREEN_SIZE_Y_PERCENT = 0.08;
	static constexpr float ITEM_BLOCK_SCREEN_SIZE_Y_PERCENT = 0.1;
	UiElement m_inventoryTop;
	UiElement m_inventoryBottom;

	static constexpr float HOTBAR_ELEMENT_COUNT = INVENTORY_X_ELEMENT_COUNT;
	static constexpr float HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT = INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT;
	UiElement m_hotbar;

	static constexpr float ITEM_COUNT_OFFSET_X_CELL_SIZE_PERCENT_Y = 0.65f;
	static constexpr float ITEM_COUNT_OFFSET_Y_CELL_SIZE_PERCENT_Y = -0.5f;


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

	TextureArray m_fontTextureArray;
};