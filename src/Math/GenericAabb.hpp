#pragma once

#include <Math/GenericVec3.hpp>

template<typename T>
struct GenericAabb
{
public:
	GenericAabb(const GenericVec3<T>& min, const GenericVec3<T>& max);
	bool contains(const GenericVec3<T>& pos) const;
	bool intersects(const GenericAabb aabb) const;

public:
	Vec3 min;
	Vec3 max;
};

template<typename T>
GenericAabb<T>::GenericAabb(const GenericVec3<T>& min, const GenericVec3<T>& max)
	: min(min)
	, max(max)
{}

template<typename T>
bool GenericAabb<T>::contains(const GenericVec3<T>& pos) const
{
	return (pos.x > min.x) && (pos.x < max.x)
		&& (pos.y > min.y) && (pos.y < max.y)
		&& (pos.z > min.z) && (pos.z < max.z);
}


template<typename T>
bool GenericAabb<T>::intersects(const GenericAabb aabb) const
{
	return (min.x <= aabb.max.x && max.x >= aabb.min.x)
		&& (min.y <= aabb.max.y && max.y >= aabb.min.y)
		&& (min.z <= aabb.max.z && max.z >= aabb.min.z);
}
