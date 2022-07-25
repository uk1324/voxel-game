#pragma once

#include <Game/PhysicsSystem.hpp>
#include <Game/Player/PlayerMovementSystem.hpp>
#include <Game/Player/PlayerInteracationSystem.hpp>
#include <Game/Blocks/BlockSystem.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Vao.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Game/Inventory/InventorySystem.hpp>
#include <Game/Rendering/RenderingSystem.hpp>
#include <Game/Console/Console.hpp>
#include <Game/EntitySystem.hpp>

class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	void update() override;

	ItemData itemData;

private:
	void registerInputActions();

private:
	bool m_isGamePaused;

	Vec3 rayStart;
	Vec3 rayEnd;

private:
	Entity m_player;

	Inventory m_inventory;

	InventorySystem m_inventorySystem;
	PlayerMovementSystem m_playerMovementSystem;
	PlayerInteractionSystem m_playerInteractionSystem;
	RenderingSystem m_renderingSystem;
	PhysicsSystem m_physicsSystem;
	BlockSystem m_blockSystem;
	EntitySystem m_entitySystem;
	Console m_console;
};