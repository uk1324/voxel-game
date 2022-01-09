#pragma once 

#include <Game/Item/ItemStack.hpp>
#include <Game/Item/ItemData.hpp>
#include <Utils/Opt.hpp>

#include <vector>

class Inventory
{
private:
	using Container = std::vector<Opt<ItemStack>>;

public:
	Inventory(size_t size);

	Opt<ItemStack> tryAdd(const ItemData& itemData, const ItemStack& itemStack);

	Opt<ItemStack>& operator[](size_t index);
	const Opt<ItemStack>& operator[](size_t index) const;

	size_t size() const;

private:
	Container m_data;
};