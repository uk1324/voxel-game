#include <Game/Player/PlayerInteracationSystem.hpp>
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
	BlockSystem& blockSystem)
{
	const Vec3& playerPos = entites.getComponent<Position>(player).value;
	const Quat& playerRot = entites.getComponent<Rotation>(player).value;

	const Vec3 rayEnd = playerPos + (playerRot * Vec3::forward * playerReachDistance);

	auto voxelRaycastHit = blockSystem.raycast(playerPos, rayEnd);
	auto entityRaycastHit = PhysicsSystem::raycast(player, playerPos, rayEnd, entites);


	if (input.isButtonDown("attack"))
	{
		if (voxelRaycastHit.has_value() 
			&& ((entityRaycastHit.has_value() == false) || (voxelRaycastHit->time < entityRaycastHit->time)))
		{
			blockSystem.trySet(voxelRaycastHit->blockPos, BlockType::Air);
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

	Debug::drawCube(Vec3(voxelRaycastHit->blockPos) + Vec3(Block::SIZE / 2.0f), Vec3(1), Vec3(1, 1, 1));
}
