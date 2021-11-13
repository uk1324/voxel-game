#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>

// TODO: Move window creation and initialization out of engine class

// TODO: Check if direction are programmed correctly by using a 3D model of axis.

// TODO: Add collider classes

// TODO: Maybe create reusable classes for non copyable resources

#include <Math/Quaternion.hpp>
#include <Math/Angles.hpp>

#include <iostream>

int main()
{
	Engine engine(60, 800, 600, "game");

	engine.addScene("game", std::unique_ptr<Scene>(new GameScene(engine)));

	engine.run("game");	
}