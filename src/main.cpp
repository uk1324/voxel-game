#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>

// https://gamedev.stackexchange.com/questions/32187/when-and-why-to-use-console-logging

int main(int argc, char* argv[])
{
	Engine engine(800, 600, "game");

	engine.addScene("game", std::unique_ptr<Scene>(new GameScene(engine)));

	engine.run("game");	
}