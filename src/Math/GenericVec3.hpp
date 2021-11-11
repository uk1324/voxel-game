#pragma once

#include <ostream>

template<typename T>
struct GenericVec3
{
public:
	GenericVec3();
	GenericVec3(T x, T y, T z);

	float length();

	bool operator== (const GenericVec3& rhs) const;
	bool operator!= (const GenericVec3& rhs) const;
	GenericVec3 operator+(const GenericVec3& rhs) const;
	GenericVec3& operator+=(const GenericVec3& rhs);
	GenericVec3 operator-(const GenericVec3& rhs) const;
	GenericVec3& operator-=(const GenericVec3& rhs);
	GenericVec3 operator*(T rhs) const;
	GenericVec3& operator*=(T rhs);
	GenericVec3 operator/(T rhs) const;
	GenericVec3& operator/=(T rhs);

	T* data();
	const T* data() const;

public:
	T x;
	T y;
	T z;
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const GenericVec3<T>& vec);

template<typename T>
T dot(const GenericVec3<T>& lhs, const GenericVec3<T>& rhs);
template<typename T>
GenericVec3<T> cross( const GenericVec3<T>& lhs, const GenericVec3<T>& rhs);


template<typename T>
GenericVec3<T>::GenericVec3()
	: x(0)
	, y(0)
	, z(0)
{}

template<typename T>
GenericVec3<T>::GenericVec3(T x, T y, T z)
	: x(x)
	, y(y)
	, z(z)
{}

template<typename T>
float GenericVec3<T>::length()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

template<typename T>
bool GenericVec3<T>::operator== (const GenericVec3& rhs) const
{
	return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
}

template<typename T>
bool GenericVec3<T>::operator!= (const GenericVec3& rhs) const
{
	return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
}

template<typename T>
GenericVec3<T> GenericVec3<T>::operator+ (const GenericVec3& rhs) const
{
	return GenericVec3(x + rhs.x, y + rhs.y, z + rhs.z);
}

template<typename T>
GenericVec3<T>& GenericVec3<T>::operator+= (const GenericVec3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

template<typename T>
GenericVec3<T> GenericVec3<T>::operator- (const GenericVec3& rhs) const
{
	return GenericVec3(x - rhs.x, y - rhs.y, z - rhs.z);
}

template<typename T>
GenericVec3<T>& GenericVec3<T>::operator-= (const GenericVec3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

template<typename T>
GenericVec3<T> GenericVec3<T>::operator* (T rhs) const
{
	return GenericVec3(x * rhs, y * rhs, z * rhs);
}

template<typename T>
GenericVec3<T>& GenericVec3<T>::operator*= (T rhs)
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

template<typename T>
GenericVec3<T> GenericVec3<T>::operator/ (T rhs) const
{
	return GenericVec3(x / rhs, y / rhs, z / rhs);
}

template<typename T>
GenericVec3<T>& GenericVec3<T>::operator/= (T rhs)
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}

template<typename T>
T* GenericVec3<T>::data()
{
	return &x;
}

template<typename T>
const T* GenericVec3<T>::data() const
{
	return &x;
}

template <typename T>
std::ostream& operator<< (std::ostream& os, const GenericVec3<T>& vec)
{
	os << '(' << vec.x << ", " << vec.y << ", " << vec.z << ')';
	return os;
}

template<typename T>
T dot(const GenericVec3<T>& lhs, const GenericVec3<T>& rhs)
{
	return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

template<typename T>
GenericVec3<T> cross(const GenericVec3<T>& lhs, const GenericVec3<T>& rhs)
{
	return GenericVec3<T>(
		(lhs.y * rhs.z) - (lhs.z * rhs.y),
		(lhs.z * rhs.x) - (lhs.x * rhs.z),
		(lhs.x * rhs.y) - (lhs.y * rhs.x)
	);
}
