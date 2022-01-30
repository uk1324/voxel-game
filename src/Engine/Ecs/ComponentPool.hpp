#pragma once

#include <Utils/SmartPointers.hpp>
#include <Engine/Ecs/ComponentIterator.hpp>

class EntityManager;

// Just used as void* for different types of components.
class Component;

// This only exists because I can't convert a type id back to a type.
// Could remove this by creating a templated function.
// componentPoolRemoveComponent(void* componentPool, Component* component)
// Then when the types are registerd store the pointers to these functions. 
// This would remove the virtual call, but it would be basically creating my own vtable.
// Though it might be faster in some cases due to data locality.
class BaseComponentPool
{
public:
	virtual ~BaseComponentPool() {};
	virtual void remove(Component* component) = 0;
};


template<typename T>
class ComponentPool : public BaseComponentPool
{
	friend class EntityManager;

public:
	ComponentIterator<T> begin();
	ComponentIterator<T> end();
	ConstComponentIterator<T> begin() const;
	ConstComponentIterator<T> end() const;
	ConstComponentIterator<T> cbegin() const;
	ConstComponentIterator<T> cend() const;

private:
	ComponentPool(EntityManager* manager, size_t maxComponents);
	~ComponentPool() override;
	
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool& operator= (const ComponentPool&) = delete;

	Entity componentGetEntity(Component& component);

	template<typename T2>
	T* add(Entity entity, T2&& component);
	void remove(T* component);
	void remove(Component* component) override;

private:
	// Can't use OwnPtr because it calls the deafult constructor.
	T* m_components;
	OwnPtr<Entity[]> m_componentEntity;
	size_t m_size;

	EntityManager* m_manager;
};

#include <Engine/Ecs/EntityManager.hpp>

template<typename T>
ComponentPool<T>::ComponentPool(EntityManager* manager, size_t maxComponents)
	: m_manager(manager)
	, m_components(reinterpret_cast<T*>(::operator new(sizeof(T)* maxComponents)))
	, m_componentEntity(makeUnique<Entity[]>(maxComponents))
	, m_size(0)
{}

template<typename T>
ComponentPool<T>::~ComponentPool()
{
	for (size_t i = 0; i < m_size; i++)
	{
		m_components[i].~T();
	}
	::operator delete(m_components);
}

template<typename T>
Entity ComponentPool<T>::componentGetEntity(Component& component)
{
	size_t componentIndex = component - m_components;
	return m_componentEntity[componentIndex];
}

template<typename T>
template<typename T2>
T* ComponentPool<T>::add(Entity entity, T2&& component)
{
	T* location = &m_components[m_size];
	new (location) T(std::forward<T2>(component));
	m_componentEntity[m_size] = entity;
	m_size++;
	return location;
}

template<typename T>
void ComponentPool<T>::remove(T* component)
{
	component->~T();
	size_t componentIndex = component - m_components;
	T* lastComponent = &m_components[m_size - 1];
	if (component != lastComponent)
	{
		new (component) T(*lastComponent);
		Entity entity = m_componentEntity[m_size - 1];
		m_componentEntity[componentIndex] = entity;
		m_manager->entityUpdateComponentReference<T>(entity, reinterpret_cast<Component*>(component));
	}
	m_size--;
}

template<typename T>
void ComponentPool<T>::remove(Component* component)
{
	remove(reinterpret_cast<T*>(component));
}

template<typename T>
ComponentIterator<T> ComponentPool<T>::begin()
{
	return ComponentIterator<T>(m_components, m_componentEntity.get());
}

template<typename T>
ComponentIterator<T> ComponentPool<T>::end()
{
	return ComponentIterator<T>(m_components + m_size, m_componentEntity.get() + m_size);
}

template<typename T>
ConstComponentIterator<T> ComponentPool<T>::begin() const
{
	return ConstComponentIterator<T>(m_components, m_componentEntity.get());
}

template<typename T>
ConstComponentIterator<T> ComponentPool<T>::end() const
{
	return ConstComponentIterator<T>(m_components + m_size, m_componentEntity.get() + m_size);
}

template<typename T>
ConstComponentIterator<T> ComponentPool<T>::cbegin() const
{
	return ComponentIterator<T>(m_components, m_componentEntity.get());
}

template<typename T>
ConstComponentIterator<T> ComponentPool<T>::cend() const
{
	return ComponentIterator<T>(m_components + m_size, m_componentEntity.get() + m_size);
}
