#pragma once

#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Game/Systems/CameraSystem.hpp>
#include <Game/Blocks/ChunkSystem.hpp>
#include <Game/Blocks/Chunk.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/VertexArray.hpp>
#include <Engine/Graphics/TextureArray.hpp>

class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	ShaderProgram shader;
	VertexArray vao;
	VertexBuffer vbo;
	Chunk chunk;

	TextureArray textureArray;

	std::unordered_map<Vec3I, ChunkStruct> chunkMesh;

	Entity* player;

	void update() override;

private:
	void registerActions();

private:
	PlayerMovementSystem playerMovementSystem;
	CameraSystem cameraSystem;
	ChunkSystem chunkSystem;
};