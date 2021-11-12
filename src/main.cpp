#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>

// TODO: Move window creation and initialization out of engine class

#include <Math/Quaternion.hpp>
#include <Math/Angles.hpp>

#include <iostream>

int main()
{
	//Quaternion q1(degToRad(90.0f), Vec3(0, 1, 0));
	//Quaternion q2 = Quaternion::identity;
	//std::cout << q1 << '\n';
	//q2 *= q1;
	//std::cout << q2.rotationMatrix() << '\n';
	//std::cout << Quaternion::identity * Quaternion::identity << '\n';

	//return 0;
	Engine engine(60, 800, 600, "game");

	engine.addScene("game", std::unique_ptr<Scene>(new GameScene(engine)));

	engine.run("game");	
}