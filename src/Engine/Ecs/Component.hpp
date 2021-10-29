#pragma once

#include <Engine/Ecs/Entity.hpp>

struct Component
{
	friend EntityManager;
	template <typename T>
	friend class ComponentPool;

protected:
	Component() = default;

public:
	Component(const Component&) = delete;
	Component& operator= (const Component&) = delete;

	template <typename T>
	T& getComponent();
	Entity& getEntity();

private:
	Entity* m_entity;
};

template<typename T>
T& Component::getComponent()
{
	return reinterpret_cast<T>(m_entity->m_components[getComponentId<T>]);
}
