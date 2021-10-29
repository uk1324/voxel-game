#include <Engine/Ecs/EntityManager.hpp>
#include <Engine/Ecs/ComponentPool.hpp>
#include <Engine/Ecs/Component.hpp>

EntityManager::EntityManager()
	: m_entityCount(0)
	, m_createdEntityCount(0)
	, m_componentPools({ nullptr })
{}

EntityManager::~EntityManager()
{
	for (size_t i = 0; i < Entity::MAX_COMPONENTS; i++)
	{
		if (m_componentPools[i] != nullptr)
		{
			delete m_componentPools[i];
		}
	}
}

Entity& EntityManager::addEntity()
{
	Entity* entity = new (&m_entites[m_entityCount]) Entity(m_createdEntityCount);
	m_entityCount++;
	m_createdEntityCount++;
	return *entity;
}

void EntityManager::removeDeadEntities()
{
	for (size_t i = 0; i < m_entityCount;)
	{
		if (m_entites[i].isAlive == false)
		{
			for (size_t j = 0; j < Entity::MAX_COMPONENTS; j++)
			{
				if (m_entites[i].m_components[j] != nullptr)
				{
					m_componentPools[j]->removeComponent(m_entites[i].m_components[j]);
				}
			}
			m_entites[i].~Entity();
			new (&m_entites[i]) Entity(std::move(m_entites[m_entityCount - 1]));
			for (size_t k = 0; k < Entity::MAX_COMPONENTS; k++)
			{
				if (m_entites[i].m_components[k] != nullptr)
				{
					m_entites[i].m_components[k]->m_entity = &m_entites[i];
				}
			}
			m_entityCount--;
		}
		else
		{
			i++;
		}
	}
}
