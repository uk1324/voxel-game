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
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0,

	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1,
	 0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1,
	-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1,

	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 2,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2,
	-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 2,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 2,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 2,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3,
	 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 3,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 3,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 3,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 3,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 4,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 4,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 4,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 4,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 5,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 5,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 5,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 5,
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
	, m_selectedInventorySlot(std::nullopt)
	, isInventoryOpen(false)
{
	m_squareTrianglesVao.bind();
	m_squareTrianglesVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, 0, sizeof(float) * 4, false));
	Gfx::VertexArray::setAttribute(1, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, sizeof(float) * 2, sizeof(float) * 4, false));

	m_cubeTrianglesVao.bind();
	m_cubeTrianglesVbo.bind();
	Gfx::VertexArray::setAttribute(0, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 3, 0, sizeof(float) * 6, false));
	Gfx::VertexArray::setAttribute(1, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 2, sizeof(float) * 3, sizeof(float) * 6, false));
	Gfx::VertexArray::setAttribute(2, Gfx::BufferLayout(Gfx::ShaderDataType::Float, 1, sizeof(float) * 5, sizeof(float) * 6, false));
}

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
	if (m_selectedHotbarSlot < 0)
		m_selectedHotbarSlot = static_cast<size_t>(selectedHotbarSlot) + static_cast<size_t>(HOTBAR_ELEMENT_COUNT);
	else
		m_selectedHotbarSlot = selectedHotbarSlot;
	m_selectedHotbarSlot %= static_cast<size_t>(HOTBAR_ELEMENT_COUNT);

	m_cursorPos = pixelScreenPosToUiScreenPos(input.mousePos());
	m_selectedInventorySlot = getInventoryItemAt(m_cursorPos);

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

	drawHotbar(inventory, blockData, itemData);
	if (isInventoryOpen)
	{
		drawInventory(inventory, blockData, itemData);
	}
}

Opt<ItemStack>& InventorySystem::heldItem(Inventory& inventory)
{
	return inventory[m_selectedHotbarSlot];
}

Opt<ItemStack> InventorySystem::handleInput(Inventory& inventory, const InputManager& input, const ItemData& itemData)
{
	if (input.isButtonDown("attack"))
	{
		if (m_selectedInventorySlot.has_value() == false)
		{
			Opt<ItemStack> droppedItem = std::move(m_heldItem);
			m_heldItem = std::nullopt;
			return droppedItem;
		}

		Opt<ItemStack>& selectedInventorySlot = inventory[m_selectedInventorySlot.value()];

		if ((m_heldItem.has_value() == false) || (selectedInventorySlot.has_value() == false))
		{
			std::swap(selectedInventorySlot, m_heldItem);
			return std::nullopt;
		}

		ItemStack& selected = selectedInventorySlot.value();
		ItemStack& held = m_heldItem.value();

		if (selected.item != held.item)
		{
			std::swap(selectedInventorySlot, m_heldItem);
			return std::nullopt;
		}
		
		const auto availableSpace = itemData[held.item].maxStackSize - selected.count;
		if (held.count > availableSpace)
		{
			held.count -= availableSpace;
			selected.count += availableSpace;
			return std::nullopt;
		}
		else
		{
			selected.count += held.count;
			m_heldItem = std::nullopt;
			return std::nullopt;
		}
	}

	if (input.isButtonDown("use"))
	{
		if (m_selectedInventorySlot.has_value() == false)
		{
			Opt<ItemStack> droppedItem = std::move(m_heldItem);
			m_heldItem = std::nullopt;
			return droppedItem;
		}

		Opt<ItemStack>& selectedInventorySlot = inventory[m_selectedInventorySlot.value()];

		if (m_heldItem.has_value() == false)
		{
			if (selectedInventorySlot.has_value())
			{
				ItemStack& selected = selectedInventorySlot.value();
				auto halfItems = selected.count / 2;

				m_heldItem = ItemStack(selected.item, selected.count - halfItems);
				selected.count = halfItems;

				if (selected.count == 0)
				{
					selectedInventorySlot = std::nullopt;
				}
			}
			return std::nullopt;
		}

		ItemStack& held = m_heldItem.value();

		if (selectedInventorySlot.has_value() == false)
		{
			selectedInventorySlot = ItemStack(held.item, 1);
			held.count -= 1;
			if (held.count == 0)
			{
				m_heldItem = std::nullopt;
			}
			return std::nullopt;
		}

		ItemStack& selected = selectedInventorySlot.value();

		if (selected.item != held.item)
		{
			std::swap(selectedInventorySlot, m_heldItem);
			return std::nullopt;
		}

		if ((itemData[selected.item].maxStackSize - selected.count) >= 1)
		{
			held.count -= 1;
			if (held.count == 0)
			{
				m_heldItem = std::nullopt;
			}
			selected.count += 1;
			return std::nullopt;
		}
	}

	return std::nullopt;
}

void InventorySystem::drawHotbar(const Inventory& inventory, const BlockData& blockData, const ItemData& itemData)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_squareTrianglesVao.bind();
	m_uiTexturedShader.use();
	m_hotbarSlotTexture.bind();
	glActiveTexture(GL_TEXTURE0);
	m_uiTexturedShader.setTexture("textureSampler", 0);

	drawUiElementTextured(m_hotbar, Vec2(HOTBAR_ELEMENT_COUNT, 1.0f));

	m_hotbarSlotSelectedTexture.bind();
	m_uiTexturedShader.setVec2("pos", getHotbarSlotPos(m_selectedHotbarSlot));
	m_uiTexturedShader.setVec2("size", Vec2(yPercentToXPercent(HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT + 0.01f), HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT + 0.01f));
	m_uiTexturedShader.setVec2("textureCoordScale", Vec2(1.0f, 1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glClear(GL_DEPTH_BUFFER_BIT);

	setupDrawBlockItem(blockData);
	for (size_t i = 0; i < HOTBAR_ELEMENT_COUNT; i++)
	{
		const Opt<ItemStack>& optItem = inventory[i];
		if (optItem.has_value())
		{
			const ItemData::Entry& itemInfo = itemData[optItem.value().item.type];
			if (itemInfo.isBlock)
			{
				drawBlockItem(blockData[itemInfo.blockType], getHotbarSlotPos(i), ITEM_BLOCK_SCREEN_SIZE_Y_PERCENT);
			}
		}
	}

	glDisable(GL_DEPTH_TEST);

	setupDrawItemNormal(itemData);
	for (size_t i = 0; i < HOTBAR_ELEMENT_COUNT; i++)
	{
		const Opt<ItemStack>& optItem = inventory[i];
		if (optItem.has_value())
		{
			const ItemData::Entry& itemInfo = itemData[optItem.value().item.type];
			if (itemInfo.isBlock == false)
			{
				drawNormalItem(itemInfo, getHotbarSlotPos(i), ITEM_NORMAL_SCREEN_SIZE_Y_PERCENT);
			}
		}
	}

	// Make a function yPercentToXpercent overload for vector
	setupDrawText();
	for (size_t i = 0; i < HOTBAR_ELEMENT_COUNT; i++)
	{
		const Opt<ItemStack>& optItem = inventory[i];
		if (optItem.has_value() && ((optItem.value().count > 1) || (optItem.value().count <= 0)))
		{
			const Vec2 offset = Vec2(ITEM_COUNT_OFFSET_X_CELL_SIZE_PERCENT_Y, ITEM_COUNT_OFFSET_Y_CELL_SIZE_PERCENT_Y) * Vec2(yPercentToXPercent(HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT), HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT);
			drawTextJustifiedRight(std::to_string(optItem.value().count), getHotbarSlotPos(i) + offset, HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT / 3.0f);
		}
	}
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);
}

void InventorySystem::drawInventory(const Inventory& inventory, const BlockData& blockData, const ItemData& itemData)
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_squareTrianglesVao.bind();
	m_uiColoredShader.use();
	m_uiColoredShader.setColor("color", Color(0.0f, 0.0f, 0.0f, 0.5f));
	m_uiColoredShader.setVec2("pos", Vec2(0.0f));
	m_uiColoredShader.setVec2("size", Vec2(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, SQUARE_TRIANGLES_VERTEX_COUNT);

	m_squareTrianglesVao.bind();
	m_uiTexturedShader.use();
	m_inventorySlotTexture.bind();
	glActiveTexture(GL_TEXTURE0);
	m_uiTexturedShader.setTexture("textureSampler", 0);

	drawUiElementTextured(m_inventoryTop, Vec2(HOTBAR_ELEMENT_COUNT, INVENTORY_Y_ELEMENT_COUNT));

	drawUiElementTextured(m_inventoryBottom, Vec2(INVENTORY_X_ELEMENT_COUNT, 1.0f));

	glClear(GL_DEPTH_BUFFER_BIT);
	setupDrawBlockItem(blockData);
	for (size_t i = 0; i < inventory.size(); i++)
	{
		const Opt<ItemStack>& optItem = inventory[i];
		if (optItem.has_value())
		{
			const ItemData::Entry& itemInfo = itemData[optItem.value().item.type];
			if (itemInfo.isBlock)
			{
				drawBlockItem(blockData[itemInfo.blockType], getInventorySlotPos(i), ITEM_BLOCK_SCREEN_SIZE_Y_PERCENT);
			}
		}
	}

	glDisable(GL_DEPTH_TEST);
	setupDrawItemNormal(itemData);
	for (size_t i = 0; i < inventory.size(); i++)
	{
		const Opt<ItemStack>& optItem = inventory[i];
		if (optItem.has_value())
		{
			const ItemData::Entry& itemInfo = itemData[optItem.value().item.type];
			if (itemInfo.isBlock == false)
			{
				drawNormalItem(itemInfo, getInventorySlotPos(i), ITEM_NORMAL_SCREEN_SIZE_Y_PERCENT);
			}
		}
	}

	setupDrawText();
	for (size_t i = 0; i < inventory.size(); i++)
	{
		const Opt<ItemStack>& optItem = inventory[i];
		if (optItem.has_value() && ((optItem.value().count > 1) || (optItem.value().count <= 0)))
		{
			const Vec2 offset = Vec2(ITEM_COUNT_OFFSET_X_CELL_SIZE_PERCENT_Y, ITEM_COUNT_OFFSET_Y_CELL_SIZE_PERCENT_Y) * Vec2(yPercentToXPercent(HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT), HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT);
			drawTextJustifiedRight(std::to_string(optItem.value().count), getInventorySlotPos(i) + offset, HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT / 3.0f);
		}
	}

	if (m_selectedInventorySlot.has_value())
	{
		m_uiColoredShader.use();
		m_uiColoredShader.setColor("color", Color(1.0f, 1.0f, 1.0f, 0.5f));
		m_uiColoredShader.setVec2("pos", getInventorySlotPos(m_selectedInventorySlot.value()));
		m_uiColoredShader.setVec2("size", Vec2(
			yPercentToXPercent(INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT - 0.1f / 8.0f),
			INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT - 0.1f / 8.0f
		));
		glDrawArrays(GL_TRIANGLES, 0, SQUARE_TRIANGLES_VERTEX_COUNT);
	}

	glEnable(GL_DEPTH_TEST);

	glClear(GL_DEPTH_BUFFER_BIT);
	if (m_heldItem.has_value())
	{
		const ItemStack& heldItem = m_heldItem.value();
		const ItemData::Entry& itemInfo = itemData[heldItem.item.type];
		if (itemInfo.isBlock)
		{
			setupDrawBlockItem(blockData);
			drawBlockItem(blockData[itemInfo.blockType], m_cursorPos, ITEM_BLOCK_SCREEN_SIZE_Y_PERCENT);
		}
		else
		{
			setupDrawItemNormal(itemData);
			drawNormalItem(itemInfo, m_cursorPos, ITEM_NORMAL_SCREEN_SIZE_Y_PERCENT);
		}

		glDisable(GL_DEPTH_TEST);
		if ((heldItem.count > 1) || (heldItem.count <= 0))
		{
			setupDrawText();
			const Vec2 offset = Vec2(ITEM_COUNT_OFFSET_X_CELL_SIZE_PERCENT_Y, ITEM_COUNT_OFFSET_Y_CELL_SIZE_PERCENT_Y) * Vec2(yPercentToXPercent(HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT), HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT);
			drawTextJustifiedRight(std::to_string(heldItem.count), m_cursorPos + offset, HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT / 3.0f);
		}
		glEnable(GL_DEPTH_TEST);
	}

	glDisable(GL_BLEND);
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
	m_itemShader.setVec2("size", Vec2(yPercentToXPercent(size), size));
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
	static const Mat4 rotation = (Quat(degToRad(-35.264), Vec3::xAxis) * Quat(degToRad(45.0f), Vec3::yAxis)).rotationMatrix();

	Mat4 mat = rotation;
	mat *= Mat4::scale(Vec3(yPercentToXPercent(size), size, size));
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
	m_fontShader.setVec2("size", Vec2(yPercentToXPercent(size), size));

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
	m_fontShader.setVec2("size", Vec2(yPercentToXPercent(size), size));

	Vec2 startPos(pos.x - size * static_cast<float>(text.length() - 1), pos.y);

	for (size_t i = 0; i < text.size(); i++)
	{
		m_fontShader.setVec2("pos", startPos + Vec2(size * i, 0.0f));
		m_fontShader.setUnsignedInt("textureIndex", text[i]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void InventorySystem::drawUiElementTextured(const UiElement& element, const Vec2& textureCoordScale)
{
	m_uiTexturedShader.setVec2("pos", element.pos);
	m_uiTexturedShader.setVec2("size", element.size);
	m_uiTexturedShader.setVec2("textureCoordScale", textureCoordScale);
	glDrawArrays(GL_TRIANGLES, 0, SQUARE_TRIANGLES_VERTEX_COUNT);
}

Opt<size_t> InventorySystem::getInventoryItemAt(Vec2 screenPos)
{
	if (contains(m_inventoryTop, screenPos))
	{
		Vec2 pos = (screenPos.negatedY() + m_inventoryTop.size + m_inventoryTop.pos)
		/ Vec2(yPercentToXPercent(INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT * 2), INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT * 2);

		return static_cast<size_t>(pos.y)* INVENTORY_X_ELEMENT_COUNT + static_cast<size_t>(pos.x) + INVENTORY_X_ELEMENT_COUNT;
	}
	else
	if (contains(m_inventoryBottom, screenPos))
	{
		Vec2 pos = (screenPos.negatedY() + m_inventoryBottom.size + m_inventoryBottom.pos)
			/ Vec2(yPercentToXPercent(INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT * 2), INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT * 2);

		return static_cast<size_t>(pos.y) * INVENTORY_X_ELEMENT_COUNT + static_cast<size_t>(pos.x);
	}

	return std::nullopt;
}

Vec2 InventorySystem::getHotbarSlotPos(size_t pos)
{
	return Vec2(
		m_hotbar.pos.x - yPercentToXPercent(0.2f * (floor(HOTBAR_ELEMENT_COUNT / 2.0f) + (fmod(HOTBAR_ELEMENT_COUNT / 2.0f, 1.0f) - 0.5f) - static_cast<float>(pos))),
		m_hotbar.pos.y
	);
}

Vec2 InventorySystem::getInventorySlotPos(size_t pos)
{
	const constexpr size_t X_ELEMENT_COUNT = static_cast<size_t>(INVENTORY_X_ELEMENT_COUNT);
	if (pos < X_ELEMENT_COUNT)
	{
		return Vec2(
			m_inventoryBottom.pos.x - yPercentToXPercent(0.2f * (floor(HOTBAR_ELEMENT_COUNT / 2.0f) + (fmod(HOTBAR_ELEMENT_COUNT / 2.0f, 1.0f) - 0.5f) - static_cast<float>(pos))),
			m_inventoryBottom.pos.y
		);
	}

	pos -= HOTBAR_ELEMENT_COUNT;
	float elementX = static_cast<float>(pos % X_ELEMENT_COUNT);
	float elementY = static_cast<float>(pos / X_ELEMENT_COUNT);

	return Vec2(
		m_inventoryTop.pos.x - yPercentToXPercent(0.2f * (floor(INVENTORY_X_ELEMENT_COUNT / 2.0f) + (fmod(INVENTORY_X_ELEMENT_COUNT / 2.0f, 1.0f) - 0.5f) - static_cast<float>(elementX))),
		m_inventoryTop.pos.y - 0.2f * elementY + 0.2
	);
}

void InventorySystem::recalculateUi()
{
	m_hotbar.size = Vec2(
		yPercentToXPercent(HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT) * HOTBAR_ELEMENT_COUNT,
		HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT
	);
	m_hotbar.pos = Vec2(
		0.0f,
		(SCREEN_BOTTOM + m_hotbar.size.y)
	);

	m_inventoryTop.size = Vec2(
		yPercentToXPercent(INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT) * INVENTORY_X_ELEMENT_COUNT,
		HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT * INVENTORY_Y_ELEMENT_COUNT
	);
	m_inventoryTop.pos = Vec2(
		0.0f,
		(DISTANCE_BETWEEN_INVENTORY_TOP_AND_INVENTORY_BOTTOM_Y_PERCENT + HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT) / 2.0
	);

	m_inventoryBottom.size = Vec2(
		yPercentToXPercent(INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT) * INVENTORY_X_ELEMENT_COUNT,
		HOTBAR_CELL_SCREEN_SIZE_Y_PERCENT
	);
	m_inventoryBottom.pos = Vec2(
		0.0f,
		m_inventoryTop.pos.y - m_inventoryTop.size.y - INVENTORY_CELL_SCREEN_SIZE_Y_PERCENT - DISTANCE_BETWEEN_INVENTORY_TOP_AND_INVENTORY_BOTTOM_Y_PERCENT
	);
}

float InventorySystem::yPercentToXPercent(float yPercent)
{
	return yPercent / m_screenSize.x * m_screenSize.y;
}

Vec2 InventorySystem::pixelScreenPosToUiScreenPos(const Vec2& pos)
{
	Vec2 convertedPos = pos;
	convertedPos /= m_screenSize * 0.5;
	convertedPos -= Vec2(1, 1);
	convertedPos.y = -convertedPos.y;
	return convertedPos;
}

bool InventorySystem::contains(const UiElement& element, const Vec2& point)
{
	return (point.x > element.pos.x - element.size.x)
		&& (point.x < element.pos.x + element.size.x)
		&& (point.y > element.pos.y - element.size.y)
		&& (point.y < element.pos.y + element.size.y);
}
