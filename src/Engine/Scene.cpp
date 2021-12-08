#include <Engine/Scene.hpp>
#include <Engine/Engine.hpp>

Scene::Scene(Engine& engine, size_t maxEntites)
	: engine(engine)
	, input(engine.window())
	, time(engine.time())
	, entityManager(maxEntites)
{}
