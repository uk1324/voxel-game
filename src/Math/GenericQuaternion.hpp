#pragma once

#include <Math/GenericVec3.hpp>
#include <Math/GenericMat4.hpp>

#include <ostream>

// TODO: write tests for length, normalization, and rotationMatrix

// https://cs.stanford.edu/~acoates/quaternion.h

template <typename T>
struct GenericQuaternion
{
public:
	GenericQuaternion();
	GenericQuaternion(T x, T y, T z, T w);
	GenericQuaternion(T angle, GenericVec3<T> axis);

	T length() const;

	GenericQuaternion normalized() const;
	void normalize();

	GenericQuaternion conjugate() const;

	GenericMat4<T> rotationMatrix() const;

	GenericQuaternion operator* (const T rhs) const;
	GenericQuaternion& operator*= (const T rhs);
	GenericQuaternion operator* (const GenericQuaternion& rhs) const;
	GenericQuaternion& operator*= (const GenericQuaternion& rhs);
	GenericVec3<T> operator* (const GenericVec3<T>& rhs) const;

public:
	static const GenericQuaternion identity;

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
GenericQuaternion<T>::GenericQuaternion(T angle, GenericVec3<T> axis)
	: x(axis.x)
	, y(axis.y)
	, z(axis.z)
	, w(angle)
{
	T c = cos(angle / 2);
	T s = sin(angle / 2);
	w = c;
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
}

template<typename T>
T GenericQuaternion<T>::length() const
{
	return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

template<typename T>
GenericQuaternion<T> GenericQuaternion<T>::normalized() const
{
	T len = length();
	if (len != 0)
	{
		return *this * (1 / len);
	}
	return *this;
}

template<typename T>
void GenericQuaternion<T>::normalize()
{
	*this = normalized();
}

template<typename T>
GenericQuaternion<T> GenericQuaternion<T>::conjugate() const
{
	return GenericQuaternion<T>(-x, -y, -z, w);
}


template<typename T>
GenericMat4<T> GenericQuaternion<T>::rotationMatrix() const
{
	return GenericMat4<T>({
		1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w, 0,
		2 * x * y + 2 * z * w, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w, 0,
		2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y, 0,
		0, 0, 0, 1
	});
}

template<typename T>
GenericQuaternion<T> GenericQuaternion<T>::operator* (const T rhs) const
{
	return GenericQuaternion<T>(x * rhs, y * rhs, z * rhs, w * rhs);
}

template<typename T>
GenericQuaternion<T>& GenericQuaternion<T>::operator*= (const T rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}

template<typename T>
GenericQuaternion<T> GenericQuaternion<T>::operator* (const GenericQuaternion& rhs) const
{
	// TODO: derive this
	return Quat(y * rhs.z - z * rhs.y + x * rhs.w + w * rhs.x,
		              z * rhs.x - x * rhs.z + y * rhs.w + w * rhs.y,
		              x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z,
		              w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
}

template<typename T>
GenericQuaternion<T>& GenericQuaternion<T>::operator*=(const GenericQuaternion& rhs)
{
	*this = *this * rhs;
	return *this;
}

template<typename T>
GenericVec3<T> GenericQuaternion<T>::operator* (const GenericVec3<T>& rhs) const
{
	// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
	GenericQuaternion<T> q = (*this * GenericQuaternion<T>(rhs.x, rhs.y, rhs.z, 0.0)) * conjugate();
	return GenericVec3<T>(q.x, q.y, q.z);
}

template<typename T>
const GenericQuaternion<T> GenericQuaternion<T>::identity(0.0, 0.0, 0.0, 1.0);

template<typename T>
std::ostream& operator<<(std::ostream& os, const GenericQuaternion<T>& quaternion)
{
	os << '(' << quaternion.x << "i + " << quaternion.y << "j + " << quaternion.z << "k + " << quaternion.w << ')';
	return os;
}
