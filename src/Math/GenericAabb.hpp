#pragma once

#include <Math/Vec3.hpp>

template<typename T>
struct GenericAabb
{
public:
	GenericAabb(const Vec3& min, const Vec3& max);
	//static 

public:
	Vec3 min;
	Vec3 max;
};

template<typename T>
GenericAabb<T>::GenericAabb(const Vec3& min, const Vec3& max)
	: min(min)
	, max(max)
{}
