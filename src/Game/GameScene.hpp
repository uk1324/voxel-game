#pragma once

#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Engine/Scene.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>
#include <Engine/Graphics/Model.hpp>


class GameScene : public Scene
{
public:
	GameScene(Engine& engine);

	ShaderProgram shader;
	unsigned int VBO, VAO;
	Model model;

	Entity* player;

	void update() override;

private:
	void registerActions();

private:
	PlayerMovementSystem playerMovementSystem;
};