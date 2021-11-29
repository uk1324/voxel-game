#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>

// TODO: Move window creation and initialization out of engine class

// TODO: Check if direction are programmed correctly by using a 3D model of axis.

// TODO: Add collider classes

// Making Position use a Vec4 might allow for simd and won't require any extra space

#include <Lang/Parsing/Parser.hpp>
#include <Lang/Parsing/Scanner.hpp>
#include <Lang/Compiling/Compiler.hpp>
#include <Utils/FileIo.hpp>
#include <Lang/Debug/AstJsonifier.hpp>

#include <iostream>

int main()
{
	// Add collider classes before the mesh compression to check if the positions actually match the real positions.

	//Lang::Scanner scanner;
	//auto tokens = scanner.parse("1 + 2 + 3; 4 + 5;");
	//Lang::Parser parser;
	//auto ast = parser.parse(tokens);

	//Lang::AstJsonifier json;
	//std::cout << json.jsonify(ast);

	//return 0;
	Engine engine(60, 800, 600, "game");

	engine.addScene("game", std::unique_ptr<Scene>(new GameScene(engine)));

	engine.run("game");	
}