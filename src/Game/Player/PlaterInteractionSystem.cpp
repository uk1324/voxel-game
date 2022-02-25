#include <Game/Player/PlayerInteracationSystem.hpp>
#include <Game/EntitySystem.hpp>
#include <Game/Blocks/BlockSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>

PlayerInteractionSystem::PlayerInteractionSystem()
	: m_lastAutoBreakTime(0.0f)
{}

#include <Game/Debug/Debug.hpp>
#include <iostream>

void PlayerInteractionSystem::update(
	Entity player,
	const ItemData& itemData,
	Opt<ItemStack>& heldItem,
	const InputManager& input,
	EntityManager& entites,
	BlockSystem& blockSystem,
	Inventory& inventory)
{
	const Vec3& playerPos = entites.getComponent<Position>(player).value;
	const Quat& playerRot = entites.getComponent<Rotation>(player).value;
	const Vec3 playerDir = playerRot * Vec3::forward;

	const Vec3 rayEnd = playerPos + (playerDir * playerReachDistance);

	auto voxelRaycastHit = blockSystem.raycast(playerPos, rayEnd);
	auto entityRaycastHit = PhysicsSystem::raycast(player, playerPos, rayEnd, entites);

	if (input.isButtonDown("attack"))
	{
		if (voxelRaycastHit.has_value() 
			&& ((entityRaycastHit.has_value() == false) || (voxelRaycastHit->time < entityRaycastHit->time)))
		{
			const auto optBlock = blockSystem.tryGet(voxelRaycastHit->blockPos);
			if (optBlock.has_value())
			{
				Vec3 velocity(0, 0.08, 0);

				Entity item = EntitySystem::spawnItemEntity(
					entites,
					Vec3(voxelRaycastHit->blockPos) + Vec3(Block::SIZE) / 2.0f,
					blockSystem.chunkSystem.blockData[optBlock->type].drop,
					velocity);

				blockSystem.trySet(voxelRaycastHit->blockPos, BlockType::Air);
			}
		}
		else if (entityRaycastHit.has_value())
		{
			Vec3& entityVel = entites.getComponent<PhysicsVelocity>(entityRaycastHit->entity).value;
			entityVel += ((rayEnd - playerPos).normalized() + Vec3::up * 3) * playerAttackKnockbackForce;
		}
	}
	else if (input.isButtonDown("use"))
	{
		if (voxelRaycastHit.has_value()
			&& ((entityRaycastHit.has_value() == false) || (voxelRaycastHit->time < entityRaycastHit->time)))
		{
			if (heldItem.has_value() && itemData[heldItem->item].isBlock)
			{
				blockSystem.trySet(
					voxelRaycastHit->blockPos + voxelRaycastHit->entryNormal.value(),
					itemData[heldItem->item].blockType
				);

				heldItem->count--;
				if (heldItem->count == 0)
					heldItem = std::nullopt;
			}
		}
	}


	for (auto& [itemEntity, itemComponent] : entites.getComponents<ItemComponent>())
	{
		const Vec3& pos = entites.getComponent<Position>(itemEntity).value;

		static constexpr float PICKUP_DISTANCE = 2.0f;
		static constexpr float PICKUP_COOLDOWN = 2.0f;
		if (((Time::currentTime() - itemComponent.spawnTime) > PICKUP_COOLDOWN)
			&& (Vec3::distanceSquared(pos, playerPos) < PICKUP_DISTANCE))
		{
			const auto rest = inventory.tryAdd(itemData, itemComponent.item);
			if (rest.has_value())
			{
				itemComponent.item = *rest;
			}
			else
			{
				itemComponent.item.count = 0;
				entites.destroyEntity(itemEntity);
			}
		}
	}

	if (input.isButtonDown("dropItem") && heldItem.has_value())
	{
		EntitySystem::spawnItemEntity(entites, playerPos, ItemStack(heldItem->item, 1), playerDir * 0.15f);
		heldItem->count -= 1;
		if (heldItem->count == 0)
			heldItem = std::nullopt;
	}

	Debug::drawCube(Vec3(voxelRaycastHit->blockPos) + Vec3(Block::SIZE / 2.0f), Vec3(1), Vec3(1, 1, 1));
}
