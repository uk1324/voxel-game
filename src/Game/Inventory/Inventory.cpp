#include <Game/Inventory/Inventory.hpp>
#include <Utils/Assertions.hpp>

Inventory::Inventory(size_t size)
	: m_data(size)
{

	if (size == 9 || size == 4 || size == 1) return;

	for (auto& item : m_data)
	{
		if (rand() % 3 == 0)
		{
			item = std::nullopt;
		}
		else
		{
			item = ItemStack(static_cast<ItemType>(rand() % (static_cast<size_t>(ItemType::Count) - 1)), rand() % 11 + 1);
		}
	}
	m_data[0] = ItemStack(ItemType::Water, 64);
}

Opt<ItemStack> Inventory::tryAdd(const ItemData& itemData, const ItemStack& itemStack)
{
	Opt<size_t> firstEmptySlot;
	 
	auto stackSize = itemStack.count;

	for (size_t i = 0; i < size(); i++)
	{
		if (m_data[i].has_value())
		{
			ItemStack& stack = m_data[i].value();
			if (stack.item == itemStack.item)
			{
				auto availableSpace = itemData[stack.item].maxStackSize - stack.count;
				if (availableSpace > 0) // Check to prevent bugs if there are more items than maxStackSize.
				{
					if (availableSpace < stackSize)
					{
						stackSize -= availableSpace;
						stack.count += availableSpace;
					}
					else
					{
						stack.count += stackSize;
						return std::nullopt;
					}
				}
			}
		}
		else if (firstEmptySlot.has_value() == false)
		{
			firstEmptySlot = i;
		}
	}

	if (firstEmptySlot.has_value())
	{
		m_data[firstEmptySlot.value()] = itemStack;
		return std::nullopt;
	}
	else
	{
		return ItemStack(itemStack.item, stackSize);
	}
}

Opt<ItemStack>& Inventory::operator[](size_t index)
{
	return m_data[index];
}

const Opt<ItemStack>& Inventory::operator[](size_t index) const
{
	return m_data[index];
}

void Inventory::set(size_t x, size_t y, GenericVec2<size_t> inventoryElementCount, Opt<ItemStack> item)
{
	m_data[getIndexAt(x, y, inventoryElementCount)] = item;
}

Opt<ItemStack>& Inventory::get(size_t x, size_t y, GenericVec2<size_t> inventoryElementCount)
{
	return m_data[getIndexAt(x, y, inventoryElementCount)];
}

const Opt<ItemStack>& Inventory::get(size_t x, size_t y, GenericVec2<size_t> inventoryElementCount) const
{
	return m_data[getIndexAt(x, y, inventoryElementCount)];
}

void Inventory::clear()
{
	for (auto& slot : m_data)
	{
		slot = std::nullopt;
	}
}

void Inventory::take(size_t index, int32_t count)
{
	auto& item = operator[](index);
	ASSERT(item.has_value() && item->count >= count);
	item->count -= count;
	if (item->count == 0)
	{
		item = std::nullopt;
	}
}

size_t Inventory::size() const
{
	return m_data.size();
}

size_t Inventory::getIndexAt(size_t x, size_t y, GenericVec2<size_t> inventoryElementCount)
{
	return y * inventoryElementCount.x + x;
}
