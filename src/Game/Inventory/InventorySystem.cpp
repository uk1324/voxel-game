#include <Game/Inventory/InventorySystem.hpp>
#include <Math/Angles.hpp>
#include <Math/Quat.hpp>

static const float squareTrianglesVertices[] = {
	// vertexPos  textureCoord
	-1.0f,  1.0F, 0.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 1.0f,
};

static const float cubeTriangleVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,

	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1,

	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 2,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 2,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2,

	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 3,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 3,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3,

	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 4,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 4,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4,

	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 5,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 5,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5,
};

static constexpr size_t CUBE_TRIANGLES_VERTEX_COUNT = 36;
static constexpr size_t SQUARE_TRIANGLES_VERTEX_COUNT = 6;

InventorySystem::InventorySystem(Scene& scene)
	: m_squareTrianglesVbo(squareTrianglesVertices, sizeof(squareTrianglesVertices))
	, m_cubeTrianglesVbo(cubeTriangleVertices, sizeof(cubeTriangleVertices))

	, m_uiTexturedShader("src/Game/Inventory/Shaders/uiTextured.vert", "src/Game/Inventory/Shaders/uiTextured.frag")
	, m_uiColoredShader("src/Game/Inventory/Shaders/uiColored.vert", "src/Game/Inventory/Shaders/uiColored.frag")
	, m_itemShader("src/Game/Inventory/Shaders/item.vert", "src/Game/Inventory/Shaders/item.frag")
	, m_3dItemShader("src/Game/Inventory/Shaders/3dItem.vert", "src/Game/Inventory/Shaders/3dItem.frag")
	, m_fontShader("src/Game/Inventory/Shaders/font.vert", "src/Game/Inventory/Shaders/font.frag")

	, m_hotbarSlotTexture("assets/textures/ui/hotbar_slot.png")
	, m_hotbarSlotSelectedTexture("assets/textures/ui/hotbar_slot_selected.png")
	, m_inventorySlotTexture("assets/textures/ui/inventory_slot.png")
	, m_fontTextureArray(32, 32, "assets/textures/ascii_font.png")

	, m_selectedHotbarSlot(0)
	, isInventoryOpen(false)
	, m_mode(Mode::Crafting)
	, m_inventoryTexture(Texture::pixelArt("assets/textures/inventory.png"))
	, m_craftedItemInventory(1)
	, m_craftingInventory(9)
{
	m_squareTrianglesVao.bind();
	m_squareTrianglesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 2, 0, sizeof(float) * 4, false));
	Vao::setAttribute(1, BufferLayout(ShaderDataType::Float, 2, sizeof(float) * 2, sizeof(float) * 4, false));

	m_cubeTrianglesVao.bind();
	m_cubeTrianglesVbo.bind();
	Vao::setAttribute(0, BufferLayout(ShaderDataType::Float, 3, 0, sizeof(float) * 6, false));
	Vao::setAttribute(1, BufferLayout(ShaderDataType::Float, 2, sizeof(float) * 3, sizeof(float) * 6, false));
	Vao::setAttribute(2, BufferLayout(ShaderDataType::Float, 1, sizeof(float) * 5, sizeof(float) * 6, false));
}

#include <iostream>

Opt<ItemStack> InventorySystem::update(Inventory& inventory, Window& window, const InputManager& input, const ItemData& itemData)
{
	if (input.isButtonDown("hotbar0")) m_selectedHotbarSlot = 0;
	if (input.isButtonDown("hotbar1")) m_selectedHotbarSlot = 1;
	if (input.isButtonDown("hotbar2")) m_selectedHotbarSlot = 2;
	if (input.isButtonDown("hotbar3")) m_selectedHotbarSlot = 3;
	if (input.isButtonDown("hotbar4")) m_selectedHotbarSlot = 4;
	if (input.isButtonDown("hotbar5")) m_selectedHotbarSlot = 5;
	if (input.isButtonDown("hotbar6")) m_selectedHotbarSlot = 6;
	if (input.isButtonDown("hotbar7")) m_selectedHotbarSlot = 7;
	if (input.isButtonDown("hotbar8")) m_selectedHotbarSlot = 8;

	int selectedHotbarSlot = static_cast<int>(m_selectedHotbarSlot) - input.scrollOffset();
	if (selectedHotbarSlot < 0)
		m_selectedHotbarSlot = static_cast<size_t>(selectedHotbarSlot) + static_cast<size_t>(m_hotbarBox.elementCount.x);
	else
		m_selectedHotbarSlot = selectedHotbarSlot;
	m_selectedHotbarSlot %= static_cast<size_t>(m_hotbarBox.elementCount.x);

	m_cursorPos = pixelScreenPosToUiScreenPos(input.mousePos());
	findSelectedSlot(inventory, m_cursorPos);

	Opt<ItemStack> droppedItem;

	if (isInventoryOpen)
	{
		droppedItem = handleInput(inventory, input, itemData);
	}

	if (input.isButtonDown("inventoryOpen"))
	{
		isInventoryOpen = !isInventoryOpen;
		if (isInventoryOpen)
		{
			window.showCursor();
		}
		else
		{
			window.hideCursor();
			if (m_heldItem.has_value())
			{
				droppedItem = inventory.tryAdd(itemData, m_heldItem.value());
			}
			m_heldItem = std::nullopt;
		}
	}

	return droppedItem;
}		

void InventorySystem::render(const Inventory& inventory, const ItemData& itemData, const BlockData& blockData, const Vec2& screenSize)
{
	if (m_screenSize != screenSize)
	{
		m_screenSize = screenSize;
		recalculateUi();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawItemBox(m_hotbarBox, inventory, 0, 9, m_hotbarSlotTexture, blockData, itemData);
	
	drawTextured(
		getItemBoxItemPos(m_hotbarBox, m_selectedHotbarSlot),
		toProportionalSize(Vec2(HALF_CELL_SIZE + 0.01f)),
		Vec2(1.0f),
		m_hotbarSlotSelectedTexture);

	if (isInventoryOpen)
	{
		// Balck half transparent background
		drawColored(Vec2(0.0f), Vec2(1.0f), Color(0.0f, 0.0f, 0.0f, 0.5f));

		drawItemBox(m_inventoryTopBox, inventory, 9, 27, m_inventorySlotTexture, blockData, itemData);
		drawItemBox(m_inventoryBottomBox, inventory, 0, 9, m_inventorySlotTexture, blockData, itemData);
		
		if (m_mode == Mode::Crafting)
		{
			drawItemBox(m_craftingBox, m_craftingInventory, 0, 9, m_inventorySlotTexture, blockData, itemData);
			drawItemBox(m_craftedItemBox, m_craftedItemInventory, 0, 1, m_inventorySlotTexture, blockData, itemData);
		}

		if (m_selectedSlot.has_value())
		{
			drawColored(m_selectedSlot->pos, toProportionalSize(Vec2(HALF_CELL_SIZE * 0.875f)), Color(1.0f, 1.0f, 1.0f, 0.5f));
		}

		if (m_heldItem.has_value())
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			drawItem(*m_heldItem, m_cursorPos, blockData, itemData);
		}
	}
}

void InventorySystem::recalculateUi()
{
	static constexpr float SCREEN_BOTTOM = -1.0f;
	m_hotbarBox = createItemBox(Vec2(0.0f, SCREEN_BOTTOM + HALF_CELL_SIZE), Vec2I(9, 1));


	static constexpr float GAP_SIZE = 0.05f;
	m_inventoryTopBox = createItemBox(Vec2(0.0f, -HALF_CELL_SIZE * 3), Vec2I(9, 3));

	m_inventoryBottomBox = createItemBox(
		Vec2(0.0, m_inventoryTopBox.pos.y - m_inventoryTopBox.halfSize.y - GAP_SIZE - HALF_CELL_SIZE),
		Vec2I(9, 1));

	m_craftingBox = createItemBox(
		Vec2(0.0, m_inventoryTopBox.pos.y + m_inventoryTopBox.halfSize.y + GAP_SIZE + 3 * HALF_CELL_SIZE),
		Vec2I(3, 3));

	m_craftedItemBox = createItemBox(
		Vec2(m_craftingBox.pos.x + m_craftingBox.halfSize.x + GAP_SIZE + HALF_CELL_SIZE, m_craftingBox.pos.y),
		Vec2I(1, 1));
}

Opt<ItemStack>& InventorySystem::heldItem(Inventory& inventory)
{
	return inventory[m_selectedHotbarSlot];
}

// onItemTaken and onItemInserted must be called after the change so the callbacks see the updated version.
Opt<ItemStack> InventorySystem::handleInput(Inventory& inventory, const InputManager& input, const ItemData& itemData)
{

	if (input.isButtonDown("attack"))
	{
		if (m_selectedSlot.has_value() == false)
		{
			Opt<ItemStack> droppedItem = std::move(m_heldItem);
			m_heldItem = std::nullopt;
			return droppedItem;
		}

		Opt<ItemStack>& selectedSlot = *m_selectedSlot->slot;

		if ((m_heldItem.has_value() == false) || (selectedSlot.has_value() == false))
		{
			if (selectedSlot.has_value())
			{
				std::swap(selectedSlot, m_heldItem);
				onItemTaken();
			}
			else if (m_heldItem.has_value() && m_selectedSlot->canPutItemsInto)
			{
				std::swap(selectedSlot, m_heldItem);
				onItemInserted();
			}

			return std::nullopt;
		}

		ItemStack& selected = selectedSlot.value();
		ItemStack& held = m_heldItem.value();

		if (selected.item != held.item)
		{
			if (m_selectedSlot->canPutItemsInto)
			{
				std::swap(selectedSlot, m_heldItem);
				onItemInserted();
				onItemTaken();
			}
			return std::nullopt;
		}
		
		if (m_selectedSlot->canPutItemsInto)
		{
			const auto availableSpace = itemData[held.item].maxStackSize - selected.count;
			if (held.count > availableSpace)
			{
				held.count -= availableSpace;
				selected.count += availableSpace;
				onItemInserted();
				return std::nullopt;
			}
			else
			{
				selected.count += held.count;
				m_heldItem = std::nullopt;
				onItemInserted();
				return std::nullopt;
			}
		}
	}

	if (input.isButtonDown("use"))
	{
		if (m_selectedSlot.has_value() == false)
		{
			Opt<ItemStack> droppedItem = std::move(m_heldItem);
			m_heldItem = std::nullopt;
			return droppedItem;
		}

		Opt<ItemStack>& selectedSlot = *m_selectedSlot->slot;

		// Currently all the actions in this if branch put items so maybe later just do
		//if (m_selectedSlot->canTakeNotAllItems == false) return std::nullopt;

		if (m_heldItem.has_value() == false)
		{
			if (selectedSlot.has_value() && m_selectedSlot->canTakeNotAllItems)
			{
				ItemStack& selected = selectedSlot.value();
				auto halfItems = selected.count / 2;

				m_heldItem = ItemStack(selected.item, selected.count - halfItems);
				selected.count = halfItems;

				if (selected.count == 0)
				{
					selectedSlot = std::nullopt;
				}
				onItemTaken();
			}
			return std::nullopt;
		}

		ItemStack& held = m_heldItem.value();

		if (selectedSlot.has_value() == false)
		{
			if (m_selectedSlot->canPutItemsInto)
			{
				selectedSlot = ItemStack(held.item, 1);
				held.count -= 1;
				if (held.count == 0)
				{
					m_heldItem = std::nullopt;
				}
				onItemInserted();
			}
			return std::nullopt;
		}

		ItemStack& selected = selectedSlot.value();

		if (selected.item != held.item)
		{
			if (m_selectedSlot->canPutItemsInto)
			{
				std::swap(selectedSlot, m_heldItem);
				onItemInserted();
				onItemTaken();
			}
			return std::nullopt;
		}

		if ((itemData[selected.item].maxStackSize - selected.count) >= 1)
		{
			if (m_selectedSlot->canPutItemsInto)
			{
				held.count -= 1;
				if (held.count == 0)
				{
					m_heldItem = std::nullopt;
				}
				selected.count += 1;
				onItemInserted();
			}
			return std::nullopt;
		}
	}

	return std::nullopt;
}

void InventorySystem::updateCrafting()
{
	m_craftingRecipeMatch = m_craftingData.checkRecipe(m_craftingInventory, GenericVec2<size_t>(3, 3));
	if (m_craftingRecipeMatch.has_value())
		m_craftedItemInventory[0] = m_craftingRecipeMatch->result();
	else
		m_craftedItemInventory[0] = std::nullopt;
}

void InventorySystem::craftItem()
{
	// This function shouldn't be called unless there is a crafting recipe. There should be an item in the result box if there was no match.
	ASSERT(m_craftingRecipeMatch.has_value());

	m_craftingData.applyRecipe(m_craftingInventory, GenericVec2<size_t>(3, 3), *m_craftingRecipeMatch);
	updateCrafting();
}

void InventorySystem::findSelectedSlot(Inventory& inventory, Vec2 cursorPos)
{
	Opt<size_t> optIndex;

#define FIND_SLOT(itemBox, itemInventory, offset, canPutItemsInto, canTakeNotAllItems, onItemTakenOrNull, onItemInsertedOrNull) \
	optIndex = getItemBoxItemIndex(itemBox, cursorPos); \
	if (optIndex.has_value()) \
	{ \
		auto index = *optIndex; \
		m_selectedSlot = ItemBoxSlot{ \
			&itemInventory[*optIndex + offset], \
			getItemBoxItemPos(itemBox, index), \
			canPutItemsInto, \
			canTakeNotAllItems, \
			onItemTakenOrNull, \
			onItemInsertedOrNull \
		}; \
		return; \
	}

	FIND_SLOT(m_inventoryBottomBox, inventory, 0, true, true, nullptr, nullptr);
	FIND_SLOT(m_inventoryTopBox, inventory, 9, true, true, nullptr, nullptr);

	if (m_mode == Mode::Crafting)
	{
		FIND_SLOT(m_craftingBox, m_craftingInventory, 0, true, true, &InventorySystem::updateCrafting, &InventorySystem::updateCrafting);
		FIND_SLOT(m_craftedItemBox, m_craftedItemInventory, 0, false, false, &InventorySystem::craftItem, nullptr);
	}

	m_selectedSlot = std::nullopt;

#undef FIND_SLOT
}

void InventorySystem::onItemInserted()
{
	if (m_selectedSlot->onItemInsertedOrNull != nullptr)
	{
		std::invoke(m_selectedSlot->onItemInsertedOrNull, this);
	}
}

void InventorySystem::onItemTaken()
{
	if (m_selectedSlot->onItemTakenOrNull != nullptr)
	{
		std::invoke(m_selectedSlot->onItemTakenOrNull, this);
	}
}

InventorySystem::ItemBox InventorySystem::createItemBox(Vec2 pos, Vec2I elementCount)
{
	return ItemBox{ Vec2(elementCount) * toProportionalSize(Vec2(HALF_CELL_SIZE)), pos, elementCount };
}

void InventorySystem::drawItemBox(
	const ItemBox& itemBox,
	const Inventory& inventory,
	size_t offset, 
	size_t count,
	const Texture& slotTexture,
	const BlockData& blockData,
	const ItemData& itemData)
{
	ASSERT(offset + count <= inventory.size());

	glDisable(GL_DEPTH_TEST);
	drawTextured(itemBox.pos, itemBox.halfSize, Vec2(itemBox.elementCount), slotTexture);

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	for (size_t i = 0; i < count; i++)
	{
		const Opt<ItemStack>& optItem = inventory[i + offset];
		if (optItem.has_value())
		{
			drawItem(*optItem, getItemBoxItemPos(itemBox, i), blockData, itemData);
		}
	}
}

Vec2 InventorySystem::getItemBoxItemPos(const ItemBox& itemBox, size_t itemIndex) const
{
	Vec2 boxTopLeftCorner = itemBox.pos - itemBox.halfSize.negatedY();
	
	Vec2 itemPos;
	itemPos.y = itemIndex / itemBox.elementCount.x;
	itemPos.x = itemIndex - itemPos.y * itemBox.elementCount.x;

	return boxTopLeftCorner + toProportionalSize(Vec2(HALF_CELL_SIZE)).negatedY() + itemPos * toProportionalSize(Vec2(CELL_SIZE)).negatedY();
}

Opt<size_t> InventorySystem::getItemBoxItemIndex(const ItemBox& itemBox, Vec2 cursorPos)
{
	if ((cursorPos.x > itemBox.pos.x - itemBox.halfSize.x)
		&& (cursorPos.x < itemBox.pos.x + itemBox.halfSize.x)
		&& (cursorPos.y > itemBox.pos.y - itemBox.halfSize.y)
		&& (cursorPos.y < itemBox.pos.y + itemBox.halfSize.y))
	{
		Vec2 topLeftCornerIncreasingDownRight = (cursorPos - itemBox.pos + itemBox.halfSize.negatedY()).negatedY();
		Vec2 elementCount = topLeftCornerIncreasingDownRight / toProportionalSize(Vec2(CELL_SIZE));

		return static_cast<size_t>(elementCount.y) * itemBox.elementCount.x + static_cast<size_t>(elementCount.x);
	}

	return std::nullopt;
}

void InventorySystem::drawItem(const ItemStack& item, Vec2 pos, const BlockData& blockData, const ItemData& itemData)
{
	const auto& itemInfo = itemData[item.item];

	if (itemInfo.isBlock)
	{
		glEnable(GL_DEPTH_TEST);
		setupDrawBlockItem(blockData);
		const auto& blockInfo = blockData[itemInfo.blockType];
		drawBlockItem(blockInfo, pos, HALF_CELL_SIZE);
	}

	glDisable(GL_DEPTH_TEST);
	if (itemInfo.isBlock == false)
	{
		setupDrawItemNormal(itemData);
		drawNormalItem(itemInfo, pos, HALF_CELL_SIZE * 0.75);
	}

	if ((item.count > 1) || (item.count <= 0))
	{
		setupDrawText();
		const Vec2 offset = Vec2(0.65f, -0.5) * toProportionalSize(Vec2(HALF_CELL_SIZE));
		drawTextJustifiedRight(std::to_string(item.count), pos + offset, HALF_CELL_SIZE / 3.0f);
	}
}


void InventorySystem::setupDrawItemNormal(const ItemData& itemData)
{
	m_squareTrianglesVao.bind();
	glActiveTexture(GL_TEXTURE0);
	itemData.textureArray.bind();
	m_itemShader.use();
	m_itemShader.setTexture("textureArraySampler", 0);
}

void InventorySystem::drawNormalItem(const ItemData::Entry& itemInfo, Vec2 pos, float size)
{
	m_itemShader.setVec2("pos", pos);
	m_itemShader.setVec2("size", toProportionalSize(Vec2(size)));
	m_itemShader.setUnsignedInt("textureIndex", itemInfo.textureIndex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void InventorySystem::setupDrawBlockItem(const BlockData& blockData)
{
	m_cubeTrianglesVao.bind();
	glActiveTexture(GL_TEXTURE0);
	blockData.textureArray.bind();
	m_3dItemShader.setTexture("textureArraySampler", 0);
	m_3dItemShader.use();
}

void InventorySystem::drawBlockItem(const BlockData::Entry& blockInfo, Vec2 pos, float size)
{
	// Isometric rotation
	static const Mat4 rotation = (Quat(degToRad(-35.264), Vec3::xAxis) * Quat(degToRad(45.0f), Vec3::yAxis)).asMatrix();

	Mat4 mat = rotation;
	mat *= Mat4::scale(Vec3(toProportionalSize(Vec2(size)).x, size, size));
	mat *= Mat4::translation(Vec3(pos.x, pos.y, 0));
	m_3dItemShader.setMat4("transform", mat);

	m_3dItemShader.setUnsignedInt("faceTextureIndex[0]", blockInfo.frontTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[1]", blockInfo.backTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[2]", blockInfo.rightTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[3]", blockInfo.leftTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[4]", blockInfo.bottomTextureIndex);
	m_3dItemShader.setUnsignedInt("faceTextureIndex[5]", blockInfo.topTextureIndex);
	glDrawArrays(GL_TRIANGLES, 0, CUBE_TRIANGLES_VERTEX_COUNT);
}

void InventorySystem::setupDrawText()
{
	m_squareTrianglesVao.bind();
	glActiveTexture(GL_TEXTURE0);
	m_fontTextureArray.bind();
	m_fontShader.use();
	m_fontShader.setTexture("textureArraySampler", 0);
}

void InventorySystem::drawTextCentered(std::string_view text, Vec2 pos, float size)
{
	m_fontShader.setVec2("size", toProportionalSize(Vec2(size)));

	Vec2 startPos(pos.x - size * (static_cast<float>(text.length() - 1) / 2.0f), pos.y);

	for (size_t i = 0; i < text.size(); i++)
	{
		m_fontShader.setVec2("pos", startPos + Vec2(size * i, 0.0f));
		m_fontShader.setUnsignedInt("textureIndex", text[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void InventorySystem::drawTextJustifiedRight(std::string_view text, Vec2 pos, float size)
{
	m_fontShader.setVec2("size", toProportionalSize(Vec2(size)));

	Vec2 startPos(pos.x - size * static_cast<float>(text.length() - 1), pos.y);

	for (size_t i = 0; i < text.size(); i++)
	{
		m_fontShader.setVec2("pos", startPos + Vec2(size * i, 0.0f));
		m_fontShader.setUnsignedInt("textureIndex", text[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void InventorySystem::drawTextured(Vec2 pos, Vec2 halfSize, Vec2 textureCoordScale, const Texture& texture)
{
	m_squareTrianglesVao.bind();
	m_uiTexturedShader.use();
	glActiveTexture(GL_TEXTURE0);
	texture.bind();
	m_uiTexturedShader.setTexture("textureSampler", 0);

	m_uiTexturedShader.setVec2("pos", pos);
	m_uiTexturedShader.setVec2("size", halfSize);
	m_uiTexturedShader.setVec2("textureCoordScale", textureCoordScale);
	glDrawArrays(GL_TRIANGLES, 0, SQUARE_TRIANGLES_VERTEX_COUNT);
}

void InventorySystem::drawColored(Vec2 pos, Vec2 halfSize, Color color)
{
	m_squareTrianglesVao.bind();
	m_uiColoredShader.use();
	m_uiColoredShader.setColor("color", color);
	m_uiColoredShader.setVec2("pos", pos);
	m_uiColoredShader.setVec2("size", halfSize);
	glDrawArrays(GL_TRIANGLES, 0, SQUARE_TRIANGLES_VERTEX_COUNT);
}

Vec2 InventorySystem::pixelScreenPosToUiScreenPos(const Vec2& pos)
{
	Vec2 convertedPos = pos;
	convertedPos /= m_screenSize * 0.5;
	convertedPos -= Vec2(1, 1);
	convertedPos.y = -convertedPos.y;
	return convertedPos;
}

Vec2 InventorySystem::toProportionalSize(const Vec2& vec) const
{
	return Vec2(vec.x / m_screenSize.x * m_screenSize.y, vec.y);
}