#pragma once

#include <Game/PhysicsSystem.hpp>
#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Game/Systems/CameraSystem.hpp>
#include <Game/Blocks/ChunkSystem.hpp>
#include <Game/Blocks/Chunk.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/VertexArray.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Game/Inventory/Inventory.hpp>
#include <Game/Rendering/RenderingSystem.hpp>

class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	void update() override;

	std::vector<Vec3I> points;
	Vec3 rayStart;
	Vec3 rayEnd;
private:
	bool m_isCursorShown;

private:
	Entity m_player;

	Inventory m_inventory;
	PlayerMovementSystem m_playerMovementSystem;
	RenderingSystem m_renderingSystem;
	ChunkSystem m_chunkSystem;
	PhysicsSystem m_physicsSystem;
};