#pragma once

#include <Game/Item/Item.hpp>

#include <stdint.h>

class ItemStack
{
public:
	ItemStack(Item item, int32_t count);

public:
	Item item;
	// not usigned to prevent overflow
	int32_t count;
};