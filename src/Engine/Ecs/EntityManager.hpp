#pragma once

#include <Engine/Ecs/Entity.hpp>
#include <Utils/UninitializedArray.hpp>

class BaseComponentPool;
template<typename T>
class ComponentPool;

class EntityManager
{
public:
	static constexpr size_t MAX_ENTITES = 1024;

public:
	EntityManager();
	~EntityManager();
	EntityManager(const EntityManager&) = delete;
	EntityManager& operator= (const EntityManager&) = delete;

	Entity& addEntity();

	void removeDeadEntities();

	template<typename T, typename ...Args>
	T& entityAddComponent(Entity& entity, Args&&... args);
	template<typename T>
	void entityRemoveComponent(Entity& entity);

	template<typename T>
	ComponentPool<T>& getComponentPool();

private:
	UninitializedArray<Entity, MAX_ENTITES> m_entites;
	size_t m_entityCount;

	size_t m_createdEntityCount;

	std::array<BaseComponentPool*, Entity::MAX_COMPONENTS> m_componentPools;
};

#include <Engine/Ecs/ComponentPool.hpp>

template<typename T, typename ...Args>
T& EntityManager::entityAddComponent(Entity& entity, Args&&... args)
{
	T* component = getComponentPool<T>().addComponent(std::forward<Args>(args)...);
	component->m_entity = &entity;
	entity.m_components[getComponentId<T>()] = reinterpret_cast<Component*>(component);
	return *component;
}

template<typename T>
void EntityManager::entityRemoveComponent(Entity& entity)
{
	getComponentPool<T>().removeComponent(entity.m_components[getComponentId<T>()]);
}

template<typename T>
ComponentPool<T>& EntityManager::getComponentPool()
{
	size_t id = getComponentId<T>();

	if (m_componentPools[id] == nullptr)
	{
		m_componentPools[id] = reinterpret_cast<BaseComponentPool*>(new ComponentPool<T>());
	}

	return *reinterpret_cast<ComponentPool<T>*>(m_componentPools[id]);
}
