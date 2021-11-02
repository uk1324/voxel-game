#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>

// Do input system next

int main()
{
	Engine engine(60, 800, 600, "game");

	engine.addScene("game", std::unique_ptr<Scene>(new GameScene(engine)));

	engine.run("game");	

    // simple comparison demo
    const std::unordered_map<int, char> example = { {1,'a'},{2,'b'} };

    auto search = example.find(2);
    if (search != example.end()) {
    }
    else {
    }
}