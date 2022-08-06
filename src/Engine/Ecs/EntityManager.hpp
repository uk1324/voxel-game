#pragma once

#include <Engine/Ecs/ComponentPool.hpp>
#include <Utils/Assertions.hpp>
#include <Utils/Opt.hpp>

#include <vector>

// TODO: Add versioning of entites.
class EntityManager
{
	template <typename T>
	friend class ComponentPool;

public:
	EntityManager(size_t maxEntites);

	template<typename T>
	void registerComponent();

	Entity createEntity();
	void destroyEntity(Entity entity);

	template<typename T>
	T& addComponent(Entity entity, T&& component);
	template<typename T>
	T& getComponent(Entity entity);
	template<typename T>
	Opt<T*> getOptComponent(Entity entity);
	template<typename T>
	const T& getComponent(Entity entity) const;
	template<typename T>
	void removeComponent(Entity entity);

	// I wanted to also implement a view like in entt that would take types and return an iterator that would return entites with all those types.
	// I decided not to do so because it might hide bugs with entites that are never iterated over because they don't have a set of components.
	// Currently if I try to get a component from an entity that doesn't have it an assertion is raised. This makes it easier to find those bugs
	// without causing undefined behaviour.
	// There are also a couple problems with the implementation. To use structured bindigs like in entt ([entity, component1, component2, ...])
	// I would need to reimplement std::tuple (structured bindings are not allowed on inherited objects).
	// I could use an alternative and return a std::pair<Entity, Components> and then do [entity, components] and then do [component1, component2, ...].
	// Another thing is how to handle the iterator increment operator. Because the iterator can only return entites with all the components
	// it would need to check if an entity doesn't have them it would need to move onto the next one, but if it doesn't check if it passed the
	// end then that would be bad. So it would need to perform 2 bounds checks. One internal and one inside the range based for loop.
	// Maybe there is a better way to do it, but I don't know how.


	template<typename T>
	ComponentPool<T>& getComponents();
	template<typename T>
	const ComponentPool<T>& getComponents() const;

	void update();

private:
	void removeDestroyedEntites();
	void removeDestrotedComponents();

	template<typename T>
	size_t getComponentIndex() const;
	template<typename T>
	void entityUpdateComponentReference(Entity entity, Component* component);

private:
	static size_t registeredTypeCount;

	template<typename T>
	static size_t getTypeId();

private:
	size_t m_maxEntites;

	size_t m_registeredComponents;

	// Type ids may not be created sequentially. For example when two different EntityManagers are
	// created and the use the same type. Type id is used as an index to convert it to a component id
	// which can be used to index other arrays. Making the other arrays packed also allows
	// iterating them linearly so for example removing all components from an entity is easier.
	std::vector<size_t> m_componentTypeIdToComponentId;

	std::vector<OwnPtr<BaseComponentPool>> m_componentPools;
	// Entity is an index into the array of arrays of components.
	std::vector<OwnPtr<Component* []>> m_entityComponent;

	std::vector<Entity> m_freeEntityIds;

	// Don't know if this is a good way to prevent iterator invalidation.
	// Maybe iterating backwards would also work.
	std::vector<Entity> m_destroyedEntites;

	struct ComponentToRemove 
	{
	public:
		// Have to store a reference so when I delete it I can set it to nullptr.
		Component** compoentReferenceLocation;
		// Can't convert the component ids back to types so I have to store the pool.
		BaseComponentPool* pool;
	};

	std::vector<ComponentToRemove> m_componentsToRemove;
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
	m_componentPools[componentIndex].reset(new ComponentPool<T>(this, m_maxEntites));
	m_entityComponent[componentIndex].reset(new Component*[m_maxEntites]);

	for (size_t i = size; i < typeId; i++)
	{
		m_componentTypeIdToComponentId[i] = -1;
	}
}

template<typename T>
T& EntityManager::addComponent(Entity entity, T&& component)
{
	using Type = std::remove_reference_t<T>;
	size_t componentIndex = m_componentTypeIdToComponentId[getTypeId<Type>()];
	auto pool = reinterpret_cast<ComponentPool<Type>*>(m_componentPools[componentIndex].get());
	Type* c = pool->add(entity, std::forward<T>(component));
	m_entityComponent[componentIndex][entity] = reinterpret_cast<Component*>(c);
	return *c;
}

template<typename T>
T& EntityManager::getComponent(Entity entity)
{
	return *reinterpret_cast<T*>(m_entityComponent[getComponentIndex<T>()][entity]);
}

template<typename T>
Opt<T*> EntityManager::getOptComponent(Entity entity)
{
	auto component = reinterpret_cast<T*>(m_entityComponent[getComponentIndex<T>()][entity]);
	if (component == nullptr)
		return std::nullopt;
	return component;
}

template<typename T>
const T& EntityManager::getComponent(Entity entity) const
{
	return *reinterpret_cast<T*>(m_entityComponent[getComponentIndex<T>()][entity]);
}

template<typename T>
void EntityManager::removeComponent(Entity entity)
{
	ComponentToRemove toRemove;
	size_t componentIndex = getComponentIndex<T>();
	toRemove.pool = m_componentPools[componentIndex].get();
	toRemove.compoentReferenceLocation = &m_entityComponent[componentIndex][entity];
	ASSERT(*toRemove.compoentReferenceLocation != nullptr);
	m_componentsToRemove.push_back(toRemove);
}

template<typename T>
ComponentPool<T>& EntityManager::getComponents()
{
	return *reinterpret_cast<ComponentPool<T>*>(m_componentPools[getComponentIndex<T>()].get());
}

template<typename T>
const ComponentPool<T>& EntityManager::getComponents() const
{
	return *reinterpret_cast<ComponentPool<T>*>(m_componentPools[getComponentIndex<T>()].get());
}

template<typename T>
size_t EntityManager::getComponentIndex() const
{
	return m_componentTypeIdToComponentId[getTypeId<T>()];
}

template<typename T>
void EntityManager::entityUpdateComponentReference(Entity entity, Component* component)
{
	m_entityComponent[getComponentIndex<T>()][entity] = component;
}

template<typename T>
size_t EntityManager::getTypeId()
{
	static const size_t id = registeredTypeCount++;
	return id;
}
