#pragma once 

#include <Game/Item/ItemStack.hpp>
#include <Game/Item/ItemData.hpp>
#include <Math/GenericVec2.hpp>
#include <Utils/Opt.hpp>

#include <vector>

// Don't create dynamic inventories to make it simpler to have stable pointers.
// If they are needed maybe use a std::deque or maybe just allocate Inventory and store an index.
class Inventory
{
private:
	using Container = std::vector<Opt<ItemStack>>;

public:
	Inventory(size_t size);

	Opt<ItemStack> tryAdd(const ItemData& itemData, const ItemStack& itemStack);

	Opt<ItemStack>& operator[](size_t index);
	const Opt<ItemStack>& operator[](size_t index) const;

	void set(size_t x, size_t y, GenericVec2<size_t> inventoryElementCount, Opt<ItemStack> item);
	Opt<ItemStack>& get(size_t x, size_t y, GenericVec2<size_t> inventoryElementCount);
	const Opt<ItemStack>& get(size_t x, size_t y, GenericVec2<size_t> inventoryElementCount) const;

	void clear();
	void take(size_t index, int32_t count);

	size_t size() const;

public:
	static size_t getIndexAt(size_t x, size_t y, GenericVec2<size_t> inventoryElementCount);

private:
	Container m_data;
};