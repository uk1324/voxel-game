#pragma once


//class 

//#pragma once
//
//#include <Engine/Ecs/Entity.hpp>
//
//struct Component
//{
//	friend EntityManager;
//	template <typename T>
//	friend class ComponentPool;
//
//protected:
//	Component() = default;
//
//	template <typename T>
//	T& getComponent();
//	Entity& getEntity();
//
//protected:
//	Component(const Component&) = default;
//	Component& operator= (const Component&) = default;
//
//private:
//	Entity* m_entity;
//};
//
//template<typename T>
//T& Component::getComponent()
//{
//	return reinterpret_cast<T>(m_entity->m_components[getComponentId<T>]);
//}
