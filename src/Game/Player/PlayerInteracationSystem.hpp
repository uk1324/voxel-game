#pragma once

#include <Game/Blocks/BlockSystem.hpp>
#include <Game/Inventory/InventorySystem.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Input/InputManager.hpp>

class PlayerInteractionSystem
{
public:
	PlayerInteractionSystem();

	void update(Entity player, const ItemData& itemData, Opt<ItemStack>& heldItem, const InputManager& input, const EntityManager& entityManager, BlockSystem& blockSystem);

private:
	float m_lastAutoBreakTime;

	float playerReachDistance = 5.0f;
};