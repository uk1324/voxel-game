#pragma once

#include <Game/Player/PlayerMovementSystem.hpp>
#include <Game/Systems/CameraSystem.hpp>
#include <TestScene/RenderingSystem.hpp>
#include <Engine/Graphics/ShaderProgram.hpp>

class TestScene : public Scene
{
public:
	TestScene(Engine& engine);

	Entity player;
	Entity entity;

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
	RenderSystem renderingSystem;
};