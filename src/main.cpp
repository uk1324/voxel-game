#include <Engine/Engine.hpp>
#include <Game/GameScene.hpp>
#include <TestScene/TestScene.hpp>

// Making Position use a Vec4 might allow for simd and won't require any extra space

// TODO: Make Vec4 and alias it to Color

// TODO: Add a way to make screenshots

#include <Lang/Parsing/Parser.hpp>
#include <Lang/Parsing/Scanner.hpp>
#include <Lang/Compiling/Compiler.hpp>
#include <Utils/FileIo.hpp>
#include <Lang/Debug/AstJsonifier.hpp>

#include <iostream>    

// Maybe remove default constructor of VertexBuffer idk.

// Maybe make a class OpenGL objects that is non copyable and has move constructor and assignment operator.

// TODO: Color class

int main()
{
	// Add assetPath function to load assets using asset path

	//Lang::Scanner scanner;
	//auto tokens = scanner.parse("1 + 2 + 3; 4 + 5;");
	//Lang::Parser parser;
	//auto ast = parser.parse(tokens);

	//Lang::AstJsonifier json;
	//std::cout << json.jsonify(ast);

	//return 0;
	Window window = Engine::init(800, 600, "game");

	Engine engine(60, window);
	/*engine.run(std::make_unique<GameScene>(engine));*/
	engine.run(std::make_unique<TestScene>(engine));

	Engine::terminate();
}