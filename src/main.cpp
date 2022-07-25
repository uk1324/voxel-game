#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>
#include <Menu/MenuScene.hpp>

// Making Position use a Vec4 might allow for simd and won't require any extra space
// TODO: Make Vec4 and alias it to Color
// TODO: Add a way to make screenshots
// Maybe make a class OpenGL objects that is non copyable and has move constructor and assignment operator.
// https://hg.sr.ht/~runevision/random-numbers-testing/browse/Assets/Implementations/HashFunctions/XXHash.cs?rev=tip
// TODO: Change components like Postion to inherit from Vec3 instead of storing it.

int main()
{
	{
		Window window = Engine::init(800, 600, "game");
		Engine engine(60, window);
		engine.run(std::make_unique<GameScene>(engine));
 	} // Engine gets destroyed before systems are terminated.
	Engine::terminate();
}