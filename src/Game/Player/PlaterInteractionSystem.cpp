#include <Game/Player/PlayerInteracationSystem.hpp>
#include <Game/Blocks/BlockSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>

PlayerInteractionSystem::PlayerInteractionSystem()
	: m_lastAutoBreakTime(0.0f)
{}

#include <Game/Debug/Debug.hpp>
#include <iostream>

void PlayerInteractionSystem::update(Entity player, const ItemData& itemData, Opt<ItemStack>& heldItem, const InputManager& input, const EntityManager& entityManager, BlockSystem& blockSystem)
{
	const Vec3& playerPos = entityManager.entityGetComponent<Position>(player).value;
	const Quat& playerRot = entityManager.entityGetComponent<Rotation>(player).value;

	const Vec3 rayEnd = playerPos + (playerRot * Vec3::forward * playerReachDistance);

	auto raycastHit = blockSystem.raycast(playerPos, rayEnd);

	if (raycastHit.has_value() == false)
		return;

	if (input.isButtonDown("attack"))
	{
		blockSystem.trySet(raycastHit->blockPos, BlockType::Air);
		std::cout << "break: " << raycastHit->blockPos << '\n';
	}
	else if (input.isButtonDown("use"))
	{
		if (raycastHit->entryNormal.has_value() == false)
			return;

		if (heldItem.has_value() && itemData[heldItem->item].isBlock)
		{
			blockSystem.trySet(
				raycastHit->blockPos + raycastHit->entryNormal.value(),
				itemData[heldItem->item].blockType
			);

			heldItem->count--;
			if (heldItem->count == 0)
				heldItem = std::nullopt;
		}
	}

	Debug::drawCube(Vec3(raycastHit->blockPos) + Vec3(Block::SIZE / 2.0f), Vec3(1), Vec3(1, 1, 1));
}
