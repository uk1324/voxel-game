#include <Engine/Ecs/EntityManager.hpp>
#include <Utils/Assertions.hpp>
#include <Log/Log.hpp>

EntityManager::EntityManager(size_t maxEntites)
	: m_maxEntites(maxEntites)
	, m_registeredComponents(0)
{
	m_freeEntityIds.reserve(maxEntites);
	for (Entity i = maxEntites - 1; i > 0; i--)
	{
		m_freeEntityIds.push_back(i);
	}
	m_freeEntityIds.push_back(0);
}

Entity EntityManager::createEntity()
{
	if (m_freeEntityIds.size() == 0)
	{
		// I could resize the vector because entity ids are stable but I would also 
		// need to update all the pointers to components.
		// A simpler solution would be to use something like a std::deque which keeps pointers stable but iteration would be a bit slower.
		LOG_FATAL("maximum entity count exceeded");
	}
	else
	{
		Entity entity = m_freeEntityIds.back();
		m_freeEntityIds.pop_back();
		for (size_t i = 0; i < m_entityComponent.size(); i++)
		{
			m_entityComponent[i][entity] = nullptr;
		}
		return entity;
	}
}

void EntityManager::destroyEntity(Entity entity)
{
	m_destroyedEntites.push_back(entity);
	m_freeEntityIds.push_back(entity);
}

size_t EntityManager::registeredTypeCount = 0;

void EntityManager::update()
{
	// This has to be in this order so if a component if removed from an entity and then the entity is destroyed
	// the component doesn't get destroyed twice.
	removeDestrotedComponents();
	removeDestroyedEntites();
}

void EntityManager::removeDestroyedEntites()
{
	for (size_t i = 0; i < m_componentTypeIdToComponentId.size(); i++)
	{
		BaseComponentPool* pool = m_componentPools[i].get();
		for (Entity entity : m_destroyedEntites)
		{
			Component* component = m_entityComponent[i][entity];
			if (component != nullptr)
			{
				pool->remove(component);
			}
		}
	}
	m_destroyedEntites.clear();
}

void EntityManager::removeDestrotedComponents()
{
	for (auto& [componentReferenceLocation, pool] : m_componentsToRemove)
	{
		// False warning this doesn't dereference a nullptr unless you remove
		// a component that an entity doesn't have.
		pool->remove(*componentReferenceLocation);
		componentReferenceLocation = nullptr;
	}
	m_componentsToRemove.clear();
}
