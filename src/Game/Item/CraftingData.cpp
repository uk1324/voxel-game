#include <Game/Item/CraftingData.hpp>
#include <Utils/Assertions.hpp>

CraftingData::CraftingData()
{
	Inventory pattern(4);
	GenericVec2<size_t> patternSize(2, 2);
	pattern.set(0, 0, patternSize, ItemStack(ItemType::Dirt, 1));
	pattern.set(1, 1, patternSize, ItemStack(ItemType::Dirt, 1));
	addRecipe(std::move(pattern), patternSize, ItemStack(ItemType::Cobblestone, 2));
}

void CraftingData::addRecipe(Inventory pattern, GenericVec2<size_t> size, ItemStack result)
{
	ASSERT(pattern.size() == (size.x * size.y));
	m_recipes.push_back(PatternRecipe{ std::move(pattern), size, result });
}

#include <iostream>

Opt<CraftingData::RecipeMatch> CraftingData::checkRecipe(const Inventory& pattern, GenericVec2<size_t> patternSize) const
{
	ASSERT(pattern.size() == (patternSize.x * patternSize.y));

	for (const auto& recipe : m_recipes)
	{
		if ((patternSize.x < recipe.size.x) || (patternSize.y < recipe.size.y))
			return std::nullopt;;


		for (size_t startX = 0; startX + recipe.size.x <= patternSize.x; startX++)
		{
			for (size_t startY = 0; startY + recipe.size.y <= patternSize.y; startY++)
			{
				for (size_t x = 0; x < patternSize.x; x++)
				{
					for (size_t y = 0; y < patternSize.y; y++)
					{
						if (x < startX || y < startY || x >= startX + recipe.size.x || y >= startY + recipe.size.y)
						{
							if (pattern.get(x, y, patternSize) != std::nullopt)
								goto nextPermutation;
							continue;
						}

						const auto& patternSlot = pattern.get(x, y, patternSize);
						const auto& recipeSlot = recipe.pattern.get(x - startX, y - startY, recipe.size);
						if ((patternSlot.has_value() == false) || (recipeSlot.has_value() == false))
						{
							if (patternSlot.has_value() != recipeSlot.has_value())
							{
								goto nextPermutation;
							}
							continue;
						}
						
						if ((patternSlot->item != recipeSlot->item) || (patternSlot->count < recipeSlot->count))
						{
							goto nextPermutation;
						}
					}
				}

				return RecipeMatch(recipe, GenericVec2<size_t>(startX, startY));
				 
				nextPermutation:
				;
			}
		}
	}

	return std::nullopt;
}

void CraftingData::applyRecipe(Inventory& crafting, GenericVec2<size_t> craftingSize, const RecipeMatch& match) const
{
	//ASSERT(match.patternRecipe.offset + match.patternRecipe.recipe->size <= craftingSize);

	//const auto& offset = match.patternRecipe.recipe->size;
	std::cout << match.patternRecipe.offset << '\n';
	for (size_t x = 0; x < match.patternRecipe.recipe->size.x; x++)
	{
		for (size_t y = 0; y < match.patternRecipe.recipe->size.y; y++)
		{
			if (match.patternRecipe.recipe->pattern.get(x, y, match.patternRecipe.recipe->size).has_value())
			{
				crafting.take(Inventory::getIndexAt(x + match.patternRecipe.offset.x, y + match.patternRecipe.offset.y, craftingSize),
					match.patternRecipe.recipe->pattern.get(x, y, match.patternRecipe.recipe->size)->count);
			}
		}
	}
}

CraftingData::RecipeMatch::RecipeMatch(const PatternRecipe& recipe, GenericVec2<size_t> offset)
	: patternRecipe({ &recipe, offset })
{}

const ItemStack& CraftingData::RecipeMatch::result()
{
	return patternRecipe.recipe->result;
}
