#pragma once

#include <Utils/SmartPointers.hpp>

using Entity = uint64_t;

class Component;

class BaseComponentPool
{
public:
	virtual ~BaseComponentPool() {};
	virtual void remove(Component* component) = 0;
};

template<typename T>
class ComponentPool : public BaseComponentPool
{
public:
	ComponentPool(size_t maxComponents);
	~ComponentPool() override;
	
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool& operator= (const ComponentPool&) = delete;

	Entity componentGetEntity(Component& component);

	T* add(Entity entity, T&& component);
	template<typename ...Args>
	T* emplace(Entity entity, Args&&... args);
	void remove(T* component);
	void remove(Component* component) override;

private:
	// Can't use OwnPtr because it calls the deafult constructor.
	T* m_components;
	OwnPtr<Entity[]> m_componentEntity;
	size_t m_size;
};

#include <Engine/Ecs/EntityManager.hpp>

template<typename T>
ComponentPool<T>::ComponentPool(size_t maxComponents)
	: m_components(reinterpret_cast<T*>(::operator new(sizeof(T)* maxComponents)))
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
T* ComponentPool<T>::add(Entity entity, T&& component)
{
	T* location = &m_components[m_size];
	new (location) T(std::forward<T>(component));
	m_componentEntity[m_size] = entity;
	m_size++;
	return location;
}

template<typename T>
template<typename ...Args>
T* ComponentPool<T>::emplace(Entity entity, Args&&... args)
{
	T* location = &m_components[m_size];
	new (location) T(std::forward<Args>(args)...);
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
		m_componentEntity[componentIndex] = m_componentEntity[m_size - 1];
	}
	m_size--;
}

template<typename T>
void ComponentPool<T>::remove(Component* component)
{
	remove(reinterpret_cast<T*>(component));
}