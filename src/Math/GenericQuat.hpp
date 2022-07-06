#pragma once

#include <Math/GenericVec3.hpp>
#include <Math/GenericMat4.hpp>

#include <ostream>

// TODO: write tests for length, normalization, and rotationMatrix

// https://cs.stanford.edu/~acoates/quaternion.h

template <typename T>
struct GenericQuat
{
public:
	// TODO: Model loader line 269.
	GenericQuat();
	// Could make this private.
	GenericQuat(T x, T y, T z, T w);
	GenericQuat(T angle, GenericVec3<T> axis);

	T length() const;

	GenericQuat normalized() const;
	void normalize();

	GenericQuat inverse() const;
	GenericQuat conjugate() const;

	GenericMat4<T> asMatrix() const;

	// Not sure if these function should take a reference or not.
	// Are there any optimziation applied for sizeof(T) < sizeof(T&).
	GenericQuat operator* (const T rhs) const;
	GenericQuat& operator*= (const T rhs);
	GenericQuat operator* (const GenericQuat& rhs) const;
	GenericQuat& operator*= (const GenericQuat& rhs);
	GenericVec3<T> operator* (const GenericVec3<T>& rhs) const;

public:
	static GenericQuat fromEuler(T x, T y, T z);
	static GenericQuat slerp(GenericQuat a, GenericQuat b, T t);

public:
	static const GenericQuat identity;

public:
	T x;
	T y;
	T z;
	T w;
};

template<typename T>
std::ostream& operator<< (std::ostream& os, const GenericQuat<T>& quaternion);

template<typename T>
GenericQuat<T>::GenericQuat()
	: x(0)
	, y(0)
	, z(0)
	, w(0)
{}

template<typename T>
GenericQuat<T>::GenericQuat(T x, T y, T z, T w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{}

template<typename T>
GenericQuat<T>::GenericQuat(T angle, GenericVec3<T> axis)
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
T GenericQuat<T>::length() const
{
	return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

template<typename T>
GenericQuat<T> GenericQuat<T>::normalized() const
{
	T len = length();
	if (len != 0)
	{
		return *this * (1 / len);
	}
	return *this;
}

template<typename T>
void GenericQuat<T>::normalize()
{
	*this = normalized();
}

template<typename T>
GenericQuat<T> GenericQuat<T>::inverse() const
{
	
}

template<typename T>
GenericQuat<T> GenericQuat<T>::conjugate() const
{
	return GenericQuat<T>(-x, -y, -z, w);
}


template<typename T>
GenericMat4<T> GenericQuat<T>::asMatrix() const
{
	return GenericMat4<T>({
		1 - 2 * y * y - 2 * z * z, 2 * x * y - 2 * z * w, 2 * x * z + 2 * y * w, 0,
		2 * x * y + 2 * z * w, 1 - 2 * x * x - 2 * z * z, 2 * y * z - 2 * x * w, 0,
		2 * x * z - 2 * y * w, 2 * y * z + 2 * x * w, 1 - 2 * x * x - 2 * y * y, 0,
		0, 0, 0, 1
	});
}

template<typename T>
GenericQuat<T> GenericQuat<T>::operator* (const T rhs) const
{
	return GenericQuat<T>(x * rhs, y * rhs, z * rhs, w * rhs);
}

template<typename T>
GenericQuat<T>& GenericQuat<T>::operator*= (const T rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	w *= rhs;
	return *this;
}

template<typename T>
GenericQuat<T> GenericQuat<T>::operator* (const GenericQuat& rhs) const
{
	// TODO: derive this
	return Quat(y * rhs.z - z * rhs.y + x * rhs.w + w * rhs.x,
		              z * rhs.x - x * rhs.z + y * rhs.w + w * rhs.y,
		              x * rhs.y - y * rhs.x + z * rhs.w + w * rhs.z,
		              w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
}

template<typename T>
GenericQuat<T>& GenericQuat<T>::operator*=(const GenericQuat& rhs)
{
	*this = *this * rhs;
	return *this;
}

template<typename T>
GenericVec3<T> GenericQuat<T>::operator* (const GenericVec3<T>& rhs) const
{
	// https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
	GenericQuat<T> q = (*this * GenericQuat<T>(rhs.x, rhs.y, rhs.z, 0)) * conjugate();
	return GenericVec3<T>(q.x, q.y, q.z);
}

template<typename T>
GenericQuat<T> GenericQuat<T>::fromEuler(T x, T y, T z)
{
	double cy = cos(z * 0.5);
	double sy = sin(z * 0.5);
	double cp = cos(y * 0.5);
	double sp = sin(y * 0.5);
	double cr = cos(x * 0.5);
	double sr = sin(x * 0.5);

	Quat(
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy,
		cr * cp * cy + sr * sp * sy
	);
}

template<typename T>
const GenericQuat<T> GenericQuat<T>::identity(0, 0, 0, 1);

template<typename T>
std::ostream& operator<<(std::ostream& os, const GenericQuat<T>& quaternion)
{
	os << '(' << quaternion.x << "i + " << quaternion.y << "j + " << quaternion.z << "k + " << quaternion.w << ')';
	return os;
}
