#pragma once

#include <Engine/Ecs/ComponentPool.hpp>

#include <vector>

class EntityManager
{
public:
	EntityManager(size_t maxEntites);

	template<typename T>
	void registerComponent();

	Entity createEntity();
	void destroyEntity(Entity entity);

	template<typename T>
	T& entityAddComponent(Entity entity, T&& component);
	template<typename T, typename ...Args>
	T& entityEmplaceComponent(Entity entity, Args&&... args);
	template<typename T>
	T& entityGetComponent(Entity entity);
	template<typename T>
	void entityRemoveComponent(Entity entity);
		
	void removeDestroyedEntites();

private:
	template<typename T>
	size_t getComponentIndex();

private:
	static size_t registeredTypeCount;

	template<typename T>
	static size_t getTypeId();

private:
	size_t m_maxEntites;

	size_t m_registeredComponents;

	std::vector<size_t> m_componentTypeIdToComponentId;
	std::vector<OwnPtr<BaseComponentPool>> m_componentPools;
	std::vector<OwnPtr<Component*[]>> m_entityComponent;

	std::vector<Entity> m_freeEntityIds;

	std::vector<Entity> m_destroyedEntites;
};

template<typename T>
void EntityManager::registerComponent()
{
	size_t typeId = getTypeId<T>();
	size_t size = m_componentTypeIdToComponentId.size();

	m_componentTypeIdToComponentId.resize(typeId + 1);
	m_componentTypeIdToComponentId[typeId] = m_registeredComponents++;

	m_componentPools.resize(m_registeredComponents);
	m_entityComponent.resize(m_registeredComponents);
	size_t componentIndex = getComponentIndex<T>();
	m_componentPools[componentIndex].reset(new ComponentPool<T>(m_maxEntites));
	m_entityComponent[componentIndex].reset(new Component*[m_maxEntites]);

	for (size_t i = size; i < typeId; i++)
	{
		m_componentTypeIdToComponentId[i] = -1;
	}
}

template<typename T>
T& EntityManager::entityAddComponent(Entity entity, T&& component)
{
	size_t componentIndex = m_componentTypeIdToComponentId[getTypeId<T>()];
	ComponentPool<T>* pool = reinterpret_cast<ComponentPool<T>*>(m_componentPools[componentIndex].get());
	T* c = pool->add(entity, std::forward<T>(component));
	m_entityComponent[componentIndex][entity] = reinterpret_cast<Component*>(c);
	return *c;
}

template<typename T, typename ...Args>
T& EntityManager::entityEmplaceComponent(Entity entity, Args&&... args)
{
	size_t componentIndex = m_componentTypeIdToComponentId[getTypeId<T>()];
	ComponentPool<T>* pool = reinterpret_cast<ComponentPool<T>*>(m_componentPools[componentIndex].get());
	T* c = pool->emplace(entity, std::forward<Args>(args)...);
	m_entityComponent[componentIndex][entity] = reinterpret_cast<Component*>(c);
	return *c;
}

template<typename T>
T& EntityManager::entityGetComponent(Entity entity)
{
	return *reinterpret_cast<T*>(m_entityComponent[getComponentIndex<T>()][entity]);
}

template<typename T>
void EntityManager::entityRemoveComponent(Entity entity)
{
	size_t componentIndex = m_componentTypeIdToComponentId[typeId];
	ComponentPool<T>* pool = reinterpret_cast<ComponentPool<T>*>(m_componentPools[componentIndex].get());
	T*& component = reinterpret_cast<T*>(m_entityComponent[componentIndex][entity]);
	pool->remove(component);
	component = nullptr;
}

template<typename T>
size_t EntityManager::getComponentIndex()
{
	return m_componentTypeIdToComponentId[getTypeId<T>()];
}

template<typename T>
size_t EntityManager::getTypeId()
{
	static const size_t id = registeredTypeCount++;
	return id;
}
