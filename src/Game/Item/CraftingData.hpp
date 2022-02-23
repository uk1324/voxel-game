#pragma once

#include <Game/Inventory/Inventory.hpp>

#include <vector>

class CraftingData
{
public:
	struct PatternRecipe
	{
		Inventory pattern;
		GenericVec2<size_t> size;
		ItemStack result;
	};

	struct CombinationRecipe
	{
		std::vector<ItemStack> items;
		ItemStack result;
	};

	struct RecipeMatch
	{
		RecipeMatch(const PatternRecipe& recipe, GenericVec2<size_t> offset);

		union
		{
			struct
			{
				// Pointer because no optional reference type.
				const PatternRecipe* recipe;
				GenericVec2<size_t> offset;
			} patternRecipe;
		};

		const ItemStack& result();
	};

public:
	CraftingData();

	Opt<CraftingData::RecipeMatch> checkRecipe(const Inventory& pattern, GenericVec2<size_t> patternSize) const;

	void applyRecipe(Inventory& crafting, GenericVec2<size_t> craftingSize, const RecipeMatch& match) const;

private:
	void addRecipe(Inventory pattern, GenericVec2<size_t> size, ItemStack result);

private:
	// Could use a hash table if there are a lot or recipes.
	// Not sure it std::map would work because it would need to compare a hash of a part of the crafting table to a hash of a
	// recipe. Every recipe could just store a hash of itself.
	std::vector<PatternRecipe> m_recipes;
};