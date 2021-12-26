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
#include <Game/Rendering/RenderingSystem.hpp>

class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	Entity player;

	void update() override;

	Gfx::CubeMap skybox;

	Gfx::ShaderProgram skyboxShader;

	std::vector<Vec3I> points;


private:

	bool isCursorShown;

private:
	PlayerMovementSystem playerMovementSystem;
	RenderingSystem m_renderingSystem;
	ChunkSystem chunkSystem;
	PhysicsSystem m_physicsSystem;
};