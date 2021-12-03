#include <Engine/Scene.hpp>
#include <Engine/Engine.hpp>

Scene::Scene(Engine& engine)
	: engine(engine)
	, input(engine.window())
	, time(engine.time())
{}
