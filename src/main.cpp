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

#include <fstream>

#include <Image/Image32.hpp>

#include <Math/Interpolation.hpp>


typedef struct RgbColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} RgbColor;

typedef struct HsvColor
{
	unsigned char h;
	unsigned char s;
	unsigned char v;
} HsvColor;

RgbColor HsvToRgb(HsvColor hsv)
{
	RgbColor rgb;
	unsigned char region, remainder, p, q, t;

	if (hsv.s == 0)
	{
		rgb.r = hsv.v;
		rgb.g = hsv.v;
		rgb.b = hsv.v;
		return rgb;
	}

	region = hsv.h / 43;
	remainder = (hsv.h - (region * 43)) * 6;

	p = (hsv.v * (255 - hsv.s)) >> 8;
	q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
	t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
	case 0:
		rgb.r = hsv.v; rgb.g = t; rgb.b = p;
		break;
	case 1:
		rgb.r = q; rgb.g = hsv.v; rgb.b = p;
		break;
	case 2:
		rgb.r = p; rgb.g = hsv.v; rgb.b = t;
		break;
	case 3:
		rgb.r = p; rgb.g = q; rgb.b = hsv.v;
		break;
	case 4:
		rgb.r = t; rgb.g = p; rgb.b = hsv.v;
		break;
	default:
		rgb.r = hsv.v; rgb.g = p; rgb.b = q;
		break;
	}

	return rgb;
}

float easeInOutQuint(float x)
{
	return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
}

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
	//Lang::Parser parser;
	//auto ast = parser.parse(tokens, sourceInfo);
	//Lang::AstJsonifier json;
	//std::cout << json.jsonify(ast);
	//return 0;

	//PerlinNoise m_noise(0);
	//Image32 img1(512, 512);
	//Image32 img2(512, 512);
	//Image32 img3(512, 512);
	//Image32 biomeMap("biome_map.png");

	//for (float x = 0; x < 512; x++)
	//{
	//	for (float y = 0; y < 512; y++)
	//	{
	//		float heightValue = m_noise.at(Vec3(x, 0.5f, y) / 100);
	//		float heightValue2 = m_noise.at(Vec3(x + 125, 0.1f, y + 2) / 100);
	//		float heightValue3 = m_noise.accumulatedOctaveAt(Vec3(x + 8743, 0.4f, y + 122));
	//		float heightMultiplier = m_noise.at(Vec3((x) * 0.01, 0.5f, (y) * 0.01));

	//		int32_t height = ((heightValue + 1) / 2.0f) * ((heightMultiplier + 1) / 2.0f);
	//		//img1.set(x, y, Color32(height, height, height));
	//		int h =  easeInOutQuint((heightValue + 1) / 2.0f) * 255;
	//		int h2 = easeInOutQuint((heightValue2 + 1) / 2.0f) * 255;
	//		heightValue3 = ((heightValue3 + 1) / 2.0f);



	//		//RgbColor c = HsvToRgb(HsvColor{ static_cast<unsigned char>(heightValue * 255), 255, 255});
	//		img1.set(x, y, biomeMap.get(h, h2));

	//	}
	//}
	//img1.saveAsPpm("1.ppm");
	//system("start ./1.ppm");
	//img2.saveAsPpm("2.ppm");
	//img3.saveAsPpm("3.ppm");
	 //Block so window is destroyed before engine is terminated.
	{
		Window window = Engine::init(800, 600, "game");
		Engine engine(60, window);
		engine.run(std::make_unique<GameScene>(engine));
 	}
	Engine::terminate();
}