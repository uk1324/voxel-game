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