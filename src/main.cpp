//
//#include <Engine/Ecs/EntityManager.hpp>
//
//struct C
//{
//public:
//	C()
//		: value(0)
//	{
//		std::cout << "deafult constructor at " << this << "\n";
//	}
//
//	C(int value)
//		: value(value)
//	{
//		std::cout << "constructor at " << this << "\n";
//	}
//
//	C(const C& o)
//		: value(o.value)
//	{
//		std::cout << "copy from " << &o << " to " << this << "\n";
//	}
//
//	C(C&& o) noexcept
//		: value(o.value)
//	{
//		std::cout << "move from " << &o << " to " << this << "\n";
//	}
//
//	~C()
//	{
//		std::cout << "destructor at " << this << " has value " << value << "\n";
//	}
//
//public:
//	int value;
//};
//
//// Compo
//
//int main()
//{
//	EntityManager manager(10);
//	manager.registerComponent<int>();
//	manager.registerComponent<C>();
//	manager.registerComponent<float>();
//
//	std::cout << "----------------\n";
//
//	//Entity e1 = manager.createEntity();
//	////Entity e2 = manager.createEntity();
//	//manager.entityEmplaceComponent<C>(e1, 5);
//	////manager.entityAddComponentForward<C>(e1, 5);
//	////manager.entityAddComponent(e2, C(3));
//	//manager.entityRemoveComponent<C>(e1);
//	////manager.destroyEntity(e1);
//
//	//// Has to be in this order
//	//manager.update();
//
//	//manager.entityEmplaceComponent<C>(e1, 6);
//	//manager.destroyEntity(e1);
//
//	//manager.update();
//
//	for (int i = 0; i < 10; i++)
//	{
//		Entity e = manager.createEntity();
//		manager.entityEmplaceComponent<C>(e, i + 1);
//	}
//
//	for (auto [entity, component] : manager.getComponentPool<C>())
//	{
//		std::cout << entity << " " << component.value << '\n';
//	}
//
//
//	//std::cout << manager.entityGetComponent<C>(e1).value << '\n';
//
//	std::cout << "----------------\n";
//}

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

#include <map>

// TODO: Add array index functions to Json::Value

int main()
{
	// Add assetPath function to load assets using asset path

	//std::string filename = "src/Lang/test.voxl";
	//std::string source = stringFromFile(filename);
	//Lang::SourceInfo sourceInfo;
	//sourceInfo.source = source;
	//sourceInfo.filename = filename;

	//Lang::Scanner scanner;
	//auto tokens = scanner.parse(sourceInfo);

	//for (size_t line : sourceInfo.lineStartOffsets)
	//{
	//	std::cout << line << '\n';
	//}

	//Lang::Parser parser;
	//auto ast = parser.parse(tokens, sourceInfo);
	//Lang::AstJsonifier json;
	//std::cout << json.jsonify(ast);

	//return 0;

	{
		Window window = Engine::init(800, 600, "game");
		Engine engine(60, window);
		engine.run(std::make_unique<GameScene>(engine));
 	}
	Engine::terminate();
}