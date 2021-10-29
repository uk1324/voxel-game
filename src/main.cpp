#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>

int main()
{
	Engine engine(60, 800, 600, "game");

	engine.addScene("game", std::unique_ptr<Scene>(new GameScene(engine)));

	engine.run("game");	
}