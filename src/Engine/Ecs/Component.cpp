#include <Engine/Ecs/Component.hpp>
#include "Component.hpp"

Entity& Component::getEntity()
{
	return *m_entity;
}