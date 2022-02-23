#pragma once

#include <Game/Item/Item.hpp>

#include <stdint.h>

class ItemStack
{
public:
	ItemStack(Item item, int32_t count);

	bool operator==(const ItemStack& other) const;
	bool operator!=(const ItemStack& other) const;

public:
	Item item;
	// not usigned to prevent overflow
	int32_t count;
};