#pragma once

#include <Game/Blocks/BlockSystem.hpp>
#include <Game/Inventory/InventorySystem.hpp>
#include <Game/PhysicsSystem.hpp>
#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Input/InputManager.hpp>

class PlayerInteractionSystem
{
public:
	PlayerInteractionSystem();

	void update(
		Entity player,
		const ItemData& itemData,
		Opt<ItemStack>& heldItem,
		const InputManager& input,
		EntityManager& entites,
		BlockSystem& blockSystem,
		Inventory& inventory);

public:
	float playerReachDistance = 5.0f;
	float playerAttackKnockbackForce = 0.05;

private:
	float m_lastAutoBreakTime;
};