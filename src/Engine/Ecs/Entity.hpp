#pragma once

#include <array>

struct Component;
class EntityManager;
class BaseComponentPool;

class Entity
{
	friend EntityManager;
	template <typename T>
	friend class ComponentPool;

public:
	static constexpr size_t MAX_COMPONENTS = 32;

public:
	template <typename T>
	T& getComponent();
	template <typename T>
	const T& getComponent() const;

private:
	Entity(size_t id);

public:
	bool isAlive;

private:
	std::array<Component*, MAX_COMPONENTS> m_components;
	const size_t m_id;
};

static size_t registeredComponentCount = 0;

template <typename T>
size_t getComponentId()
{
	static const size_t id = registeredComponentCount++;
	return id;
}

template<typename T>
T& Entity::getComponent()
{
	return *reinterpret_cast<T*>(m_components[getComponentId<T>()]);
}

template<typename T>
const T& Entity::getComponent() const
{
	return *reinterpret_cast<T*>(m_components[getComponentId<T>()]);
}