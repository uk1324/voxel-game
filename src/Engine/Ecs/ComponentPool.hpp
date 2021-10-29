#pragma once

#include <Engine/Ecs/EntityManager.hpp>

#include <new>

class BaseComponentPool
{
public:
	virtual ~BaseComponentPool() {};
	virtual void removeComponent(Component* component) = 0;
};

template<typename T>
class ComponentPool : BaseComponentPool
{
public:
	ComponentPool();
	~ComponentPool() override;
	ComponentPool(const ComponentPool&) = delete;
	ComponentPool& operator= (const ComponentPool&) = delete;

	template <typename ...Args>
	T* addComponent(Args&&... args);
	void removeComponent(T* component);
	void removeComponent(Component* component) override;

	T* begin();
	T* end();
	const T* cbegin() const;
	const T* cend() const;

private:
	UninitializedArray<T, EntityManager::MAX_ENTITES> m_data;
	size_t m_size;
};

template <typename T>
ComponentPool<T>::ComponentPool()
	: m_size(0)
{}

template<typename T>
ComponentPool<T>::~ComponentPool()
{
	for (size_t i = 0; i < m_size; i++)
	{
		m_data[i].~T();
	}
}

template<typename T>
template<typename ...Args>
T* ComponentPool<T>::addComponent(Args && ...args)
{
	T* component = &m_data[m_size];
	new (&m_data[m_size]) T(std::forward<Args>(args)...);
	m_size++;
	return component;
}

template<typename T>
void ComponentPool<T>::removeComponent(T* component)
{
	component->~T();
	if (component != &m_data[m_size - 1])
	{
		new (component) T(std::move(m_data[m_size - 1]));
		component->m_entity->m_components[getComponentId<T>()] = component;
	}
	m_size--;
}

template<typename T>
void ComponentPool<T>::removeComponent(Component* component)
{
	removeComponent(reinterpret_cast<T*>(component));
}

template<typename T>
T* ComponentPool<T>::begin()
{
	return m_data.data();
}

template<typename T>
T* ComponentPool<T>::end()
{
	return m_data.data() + m_size;
}

template<typename T>
const T* ComponentPool<T>::cbegin() const
{
	return m_data();
}

template<typename T>
const T* ComponentPool<T>::cend() const
{
	return m_data() + m_size;
}
