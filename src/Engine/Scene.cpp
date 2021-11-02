#include <Engine/Scene.hpp>
#include <Engine/Engine.hpp>

Scene::Scene(Engine& engine)
	: engine(engine)
	, input(engine.window())
{}

void Scene::changeScene(std::string_view name)
{
	engine.changeScene(name);
}
