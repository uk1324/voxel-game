#pragma once

#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Game/Systems/CameraSystem.hpp>
#include <Game/Blocks/ChunkSystem.hpp>
#include <Game/Blocks/Chunk.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/VertexArray.hpp>
#include <Engine/Graphics/TextureArray.hpp>
#include <Engine/Graphics/CubeMap.hpp>

class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	Gfx::CubeMap skybox;

	Gfx::ShaderProgram skyboxShader;

	Gfx::ShaderProgram shader;

	Gfx::TextureArray textureArray;

	Entity player;

	void update() override;

private:

private:
	PlayerMovementSystem playerMovementSystem;
	CameraSystem cameraSystem;
	ChunkSystem chunkSystem;
};