#pragma once

#include <Math/GenericVec3.hpp>

template<typename T>
struct GenericAabb
{
public:
	GenericAabb(const GenericVec3<T>& min, const GenericVec3<T>& max);
	bool contains(const GenericVec3<T>& pos);

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
bool GenericAabb<T>::contains(const GenericVec3<T>& pos)
{
	return (pos.x > min.x) && (pos.x < max.x)
		&& (pos.y > min.y) && (pos.y < max.y)
		&& (pos.z > min.z) && (pos.z < max.z);
}
