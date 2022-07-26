#pragma once

// Item could store a pointer to a virtual class ItemProperties that would contain additional data.
// it may have an interface like getDamageMultiplier or isEnchaned so every items even not weapons could be enchanted like in miencraft.

// There is no Null item type because then there might be null item entites and there would need to be checks in all systems to handle 
// them in all systems like inventory and combining items entity stacks when they are close.
// I think a better way is to use std::optional so there can be empty items only when needed. This also ensures that the values are checked.
enum class ItemType
{
	DiamondSword,
	Stone,
	Grass,
	Dirt,
	WoodenPlanks,
	Cobblestone,
	Debug,
	Water,
	Count
};

class ItemProperties;

class Item
{
public:
	Item(ItemType type);

	bool operator==(const Item& item) const;
	bool operator!=(const Item& item) const;

public:
	ItemType type;
};