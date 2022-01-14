#pragma once

#include <Game/PhysicsSystem.hpp>
#include <Game/Player/PlayerMovementSystem.hpp>
#include <Game/Player/PlayerInteracationSystem.hpp>
#include <Game/Blocks/BlockSystem.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/VertexArray.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Game/Inventory/InventorySystem.hpp>
#include <Game/Rendering/RenderingSystem.hpp>

class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	void update() override;

	std::vector<Vec3I> points;
	ItemData itemData;
	Vec3 rayStart;
	Vec3 rayEnd;

private:
	void registerInputActions();

private:
	bool m_isGamePaused;

private:
	Entity m_player;

	Inventory m_inventory;

	InventorySystem m_inventorySystem;
	PlayerMovementSystem m_playerMovementSystem;
	PlayerInteractionSystem m_playerInteractionSystem;
	RenderingSystem m_renderingSystem;
	PhysicsSystem m_physicsSystem;
	BlockSystem m_blockSystem;
};