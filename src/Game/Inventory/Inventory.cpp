#include <Game/Inventory/Inventory.hpp>

Inventory::Inventory(size_t size)
	: m_data(size)
{

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

	m_data[0] = ItemStack(ItemType::DiamondSword, 1);
	m_data[1] = ItemStack(ItemType::DiamondSword, 11);
	m_data[2] = ItemStack(ItemType::DiamondSword, 111);
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

size_t Inventory::size() const
{
	return m_data.size();
}
