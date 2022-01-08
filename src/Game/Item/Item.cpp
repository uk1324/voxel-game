#include <Game/Item/Item.hpp>

Item::Item(ItemType type)
	: type(type)
{}

bool Item::operator==(const Item & item) const
{
	return type == item.type;
}

bool Item::operator!=(const Item& item) const
{
	return type != item.type;
}
