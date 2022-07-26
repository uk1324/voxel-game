#include <Game/Player/PlayerInteracationSystem.hpp>
#include <Game/EntitySystem.hpp>
#include <Game/Blocks/BlockSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>
#include <Game/Debug/Debug.hpp>
#include <Math/Aabb.hpp>

PlayerInteractionSystem::PlayerInteractionSystem()
	: m_lastAutoBreakTime(0.0f)
{}

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
	const auto& collider = entites.getComponent<PhysicsAabbCollider>(player);
	const auto& colliderPos = playerPos + collider.centerOffset;

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
				auto velocity = Vec3::randomInRange(Vec3(-0.04, 0.08, -0.04), Vec3(0.04, 0.09, 0.04));

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
			&& ((entityRaycastHit.has_value() == false) || (voxelRaycastHit->time < entityRaycastHit->time))
			&& heldItem.has_value()
			&& itemData[heldItem->item].isBlock
			&& voxelRaycastHit->entryNormal.has_value())
		{
			// TODO this test has to be applied to all entites with a collider not just the player.
			const auto playerAabb = Aabb(colliderPos - collider.halfSize, colliderPos + collider.halfSize);
			const auto blockPos = voxelRaycastHit->blockPos + *voxelRaycastHit->entryNormal;
			const auto blockAAbb = Aabb(Vec3(blockPos), Vec3(blockPos) + Vec3(1));

			if (playerAabb.intersects(blockAAbb) == false )
			{
				blockSystem.trySet( 
					blockPos,
					itemData[heldItem->item].blockType
				);

				heldItem->count--;
				if (heldItem->count == 0)
					heldItem = std::nullopt;
			}
		}  
	}

	auto pickupAabb = Aabb(colliderPos - collider.halfSize - Vec3(1, 0.5, 1), colliderPos + collider.halfSize + Vec3(1, 0.5, 1));
	for (auto& [itemEntity, itemComponent] : entites.getComponents<ItemComponent>())
	{
		const Vec3& pos = entites.getComponent<Position>(itemEntity).value;

		static constexpr float PICKUP_DISTANCE = 2.0f;
		static constexpr float PICKUP_COOLDOWN = 0.5f;
		if (((Time::currentTime() - itemComponent.spawnTime) > PICKUP_COOLDOWN)
			//&& (Vec3::distanceSquared(pos, playerPos) < PICKUP_DISTANCE))
			&& (pickupAabb.contains(pos)))
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

	if (voxelRaycastHit.has_value())
	{
		const auto block = blockSystem.tryGet(voxelRaycastHit->blockPos);
		if (!(block.has_value() && (block->type == BlockType::Water)))
			Debug::drawCube(Vec3(voxelRaycastHit->blockPos) + Vec3(Block::SIZE / 2.0f), Vec3(1), Vec3(1, 1, 1));
	}
}
