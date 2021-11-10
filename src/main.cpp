#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>

// Move window creation and initialization out of engine class

#include <Math/Quaternion.hpp>
#include <Math/Angles.hpp>
#include <Math/Vec3.hpp>
#include <Math/Mat4.hpp>

#include <iostream>

int main()
{
	//Mat4 m = {
	//	1, 2, 3, 4,
	//	5, 6, 7, 8,
	//	9, 10, 11, 12,
	//	13, 14, 15, 16
	//};
	//Mat4 m;
	//m(3, 0) = 1;
	//m(3, 1) = 2;
	//m(3, 2) = 3;
	//m(3, 3) = 4;

	Mat4 m = Mat4::identity();
	//m = m * 2;
	Vec3 v(1, 2, 3);
	//m = translate(m, v);
	m = scale(m, Vec3(2, 2, 2));

	v = m * v;

	std::cout << m << '\n';
	std::cout << v << '\n';

	for (size_t i = 0; i < 16; i++)
	{
		std::cout << m.data()[i] << ' ';
	}

	//Mat4 m;
	//m.set(3, 3, 3);
	//m.set(3, 2, 2);
	//m.set(3, 1, 1);
	//m.set(3, 0, 0);
	//std::cout << m;

	//for (size_t i = 0; i < 16; i++)
	//{
	//	std::cout << m.data()[i] << ' ';
	//}

	//std::cout << degToRad(180.0);

	//Quaternion q(1, 2, 3, 4);
	//std::cout << q;

	return 0;
	Engine engine(60, 800, 600, "game");

	engine.addScene("game", std::unique_ptr<Scene>(new GameScene(engine)));

	engine.run("game");	
}