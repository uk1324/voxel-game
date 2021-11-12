#pragma once

#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>

class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	ShaderProgram shader;
	unsigned int VBO, VAO;

	Entity* player;

	void update() override;

private:
	void registerActions();

private:
	PlayerMovementSystem playerMovementSystem;
};