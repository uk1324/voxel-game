#include <Game/Item/ItemStack.hpp>

ItemStack::ItemStack(Item item, int32_t count)
	: item(item)
	, count(count)
{}

bool ItemStack::operator==(const ItemStack& other) const
{
	return (count == other.count) && (item == other.item);
}

bool ItemStack::operator!=(const ItemStack& other) const
{
	return (count != other.count) || (item != other.item);
}
