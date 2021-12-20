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

	static constexpr float point[] = { 0.0, 0.0, 0.0 };
	Gfx::VertexBuffer vbo;
	Gfx::VertexArray vao;
	Gfx::ShaderProgram pointShader;

	std::vector<Vec3> points;

	//static constexpr float point[] = { 0.0 };
	//Gfx::VertexArray vao;
	//Gfx::VertexArray vbo;


	Entity player;

	void update() override;

private:

private:
	PlayerMovementSystem playerMovementSystem;
	CameraSystem cameraSystem;
	ChunkSystem chunkSystem;
};