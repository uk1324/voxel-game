#include <Engine/Ecs/Entity.hpp>

Entity::Entity(size_t id)
	: m_components({ nullptr })
	, m_id(id)
	, isAlive(true)
{}