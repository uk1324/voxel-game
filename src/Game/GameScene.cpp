#include <Game/GameScene.hpp>

GameScene::GameScene(Engine& engine)
	: Scene(engine)
{}

void GameScene::update()
{
	meshRenderSystem.update(*this);
}