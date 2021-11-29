#pragma once

#include <Math/Vec3.hpp>

template<typename T>
struct GenericBox
{
public:
	GenericBox(const GenericVec3<T>& position, const GenericVec3<T>& size);
	//GenericBox(GenericVec3<)

	bool contains(const GenericVec3<T>& point);

public:
	GenericVec3<T> position;
	GenericVec3<T> size;
};

template<typename T>
GenericBox<T>::GenericBox(const GenericVec3<T>& position, const GenericVec3<T>& size)
	: position(position)
	, size(size)
{}

template<typename T>
bool GenericBox<T>::contains(const GenericVec3<T>& point)
{
	return ((point.x > position.x) && (point.x < (position.x + size.x)))
		&& ((point.y > position.y) && (point.y < (position.y + size.y)))
		&& ((point.z > position.z) && (point.z < (position.z + size.z)));
}
