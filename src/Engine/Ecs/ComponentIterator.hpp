#pragma once

#include <algorithm>

using Entity = uint64_t;

template<typename T>
struct ComponentIterator
{
public:
	ComponentIterator(T* component, Entity* entity);

    ComponentIterator& operator++();
    ComponentIterator operator++(int);
    ComponentIterator& operator--();
    ComponentIterator operator--(int);
    std::pair<Entity&, T&> operator*() const;
    bool operator ==(const ComponentIterator& other) const;
    bool operator !=(const ComponentIterator& other) const;

private:
	T* m_component;
	Entity* m_entity;
};

template<typename T>
struct ConstComponentIterator
{
public:
    ConstComponentIterator(const T* component, const Entity* entity);

    ConstComponentIterator& operator++();
    ConstComponentIterator operator++(int);
    ConstComponentIterator& operator--();
    ConstComponentIterator operator--(int);
    std::pair<const Entity&, const T&> operator*() const;
    bool operator ==(const ConstComponentIterator& other) const;
    bool operator !=(const ConstComponentIterator& other) const;

private:
    const T* m_component;
    const Entity* m_entity;
};

template<typename T>
ComponentIterator<T>::ComponentIterator(T* component, Entity* entity)
    : m_component(component)
    , m_entity(entity)
{}

template<typename T>
ComponentIterator<T>& ComponentIterator<T>::operator++()
{
    ++m_component;
    ++m_entity;
    return *this;
}

template<typename T>
ComponentIterator<T> ComponentIterator<T>::operator++(int)
{
    ComponentIterator<T> copy = *this;
    ++(*this);
    return copy;
}

template<typename T>
ComponentIterator<T>& ComponentIterator<T>::operator--()
{
    --m_component;
    --m_entity;
    return *this;
}

template<typename T>
ComponentIterator<T> ComponentIterator<T>::operator--(int)
{
    ComponentIterator<T> copy = *this;
    --(*this);
    return copy;
}

template<typename T>
std::pair<Entity&, T&> ComponentIterator<T>::operator*() const
{
    return std::pair<Entity&, T&>(*m_entity, *m_component);
}

template<typename T>
bool ComponentIterator<T>::operator==(const ComponentIterator& other) const
{
    return m_component == other.m_component;
}

template<typename T>
bool ComponentIterator<T>::operator!=(const ComponentIterator& other) const
{
    return m_component != other.m_component;
}

template<typename T>
ConstComponentIterator<T>::ConstComponentIterator(const T* component, const Entity* entity)
    : m_component(component)
    , m_entity(entity)
{}

template<typename T>
ConstComponentIterator<T>& ConstComponentIterator<T>::operator++()
{
    ++m_component;
    ++m_entity;
    return *this;
}

template<typename T>
ConstComponentIterator<T> ConstComponentIterator<T>::operator++(int)
{
    ConstComponentIterator<T> copy = *this;
    ++(*this);
    return copy;
}

template<typename T>
ConstComponentIterator<T>& ConstComponentIterator<T>::operator--()
{
    --m_component;
    --m_entity;
    return *this;
}

template<typename T>
ConstComponentIterator<T> ConstComponentIterator<T>::operator--(int)
{
    ConstComponentIterator<T> copy = *this;
    --(*this);
    return copy;
}

template<typename T>
std::pair<const Entity&, const T&> ConstComponentIterator<T>::operator*() const
{
    return std::pair<const Entity&, const T&>(*m_entity, *m_component);
}

template<typename T>
bool ConstComponentIterator<T>::operator==(const ConstComponentIterator& other) const
{
    return m_component == other.m_component;
}

template<typename T>
bool ConstComponentIterator<T>::operator!=(const ConstComponentIterator& other) const
{
    return m_component != other.m_component;
}
