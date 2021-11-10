#include <Game/GameScene.hpp>
#include <Game/Actions/JumpAction.hpp>
#include <Game/Actions/CameraRotateAction.hpp>
#include <Game/Actions/AttackAction.hpp>

GameScene::GameScene(Engine& engine)
	: Scene(engine)
{
	input.registerKeyboardAction(KeyCode::Space, std::unique_ptr<ButtonAction>(new JumpAction));
	input.registerMouseMoveAction(std::unique_ptr<CameraRotateAction>(new CameraRotateAction));
	input.registerMouseButtonAction(MouseButton::Left, std::unique_ptr<AttackAction>(new AttackAction));
}

void GameScene::update()
{
	
}