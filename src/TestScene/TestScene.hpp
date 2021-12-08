#pragma once

#include <Game/Systems/PlayerMovementSystem.hpp>
#include <Game/Systems/CameraSystem.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>

class TestScene : public Scene
{
public:
	TestScene(Engine& engine);

	Entity player;

	Gfx::ShaderProgram shader;

	void update() override;

private:
	void registerInputActions();
	void registerComponents();
	void createPlayer();
	void setupGraphics();
	
	void updateCamera();

private:
	PlayerMovementSystem playerMovementSystem;
	CameraSystem cameraSystem;
};