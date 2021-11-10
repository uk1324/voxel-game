#pragma once

#include <ostream>

template <typename T>
struct GenericQuaternion
{
public:
	GenericQuaternion();
	GenericQuaternion(T x, T y, T z, T w);

	void normalize();
	GenericQuaternion normalized() const;

	GenericQuaternion operator* (const GenericQuaternion& rhs) const;
	GenericQuaternion& operator*= (const GenericQuaternion& rhs);

public:
	T x;
	T y;
	T z;
	T w;
};

template<typename T>
std::ostream& operator<< (std::ostream& os, const GenericQuaternion<T>& quaternion);

template<typename T>
GenericQuaternion<T>::GenericQuaternion()
	: x(0)
	, y(0)
	, z(0)
	, w(0)
{}

template<typename T>
GenericQuaternion<T>::GenericQuaternion(T x, T y, T z, T w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{}

template<typename T>
void GenericQuaternion<T>::normalize()
{
}

template<typename T>
GenericQuaternion<T> GenericQuaternion<T>::normalized() const
{
	return GenericQuaternion();
}

template<typename T>
GenericQuaternion<T> GenericQuaternion<T>::operator* (const GenericQuaternion& rhs) const
{
	return GenericQuaternion(
		w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
		w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
		w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
		w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
	);
}

template<typename T>
GenericQuaternion<T>& GenericQuaternion<T>::operator*=(const GenericQuaternion& rhs)
{
	*this = *this * rhs;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const GenericQuaternion<T>& quaternion)
{
	os << '(' << quaternion.x << "i + " << quaternion.y << "j + " << quaternion.z << "k + " << quaternion.w << ')';
	return os;
}
