#pragma once

#include <Math/GenericVec2.hpp>

#include <ostream>
#include <random>

template<typename T>
struct GenericVec3
{
public:
	GenericVec3();
	explicit GenericVec3(T all);
	GenericVec3(T x, T y, T z);
	template<typename U>
	explicit GenericVec3(const GenericVec3<U>& other);

	float length() const;
	float lengthSquared() const;

	//GenericVec3 apply
	GenericVec3<T> applied(T (*function)(T)) const;
	GenericVec3<T>& apply(T(*function)(T));

	// TODO:
	// Make more functions chainable

	GenericVec3<T> normalized() const;
	void normalize();

	bool operator== (const GenericVec3& rhs) const;
	bool operator!= (const GenericVec3& rhs) const;
	GenericVec3 operator+(const GenericVec3& rhs) const;
	GenericVec3& operator+=(const GenericVec3& rhs);
	GenericVec3 operator-(const GenericVec3& rhs) const;
	GenericVec3& operator-=(const GenericVec3& rhs);
	GenericVec3 operator*(T rhs) const;
	GenericVec3& operator*=(T rhs);
	GenericVec3 operator*(const GenericVec3& rhs) const;
	GenericVec3 operator/(T rhs) const;
	GenericVec3 operator/(const GenericVec3& rhs) const;
	GenericVec3& operator/=(T rhs);
	GenericVec3 operator-() const;
	// To make this safer could make an axis enum and an iterator for them.
	T& operator[](size_t i);
	const T& operator[](size_t i) const;

	GenericVec2<T> xz() const;

	T* data();
	const T* data() const;

public:
	static float distance(const GenericVec3<T>& a, const GenericVec3<T>& b);
	static float distanceSquared(const GenericVec3<T>& a, const GenericVec3<T>& b);
	static GenericVec3 randomInRange(const GenericVec3<T>& min, const GenericVec3<T>& max);

public:
	static const GenericVec3<T> up;
	static const GenericVec3<T> down;
	static const GenericVec3<T> left;
	static const GenericVec3<T> right;
	static const GenericVec3<T> forward;
	static const GenericVec3<T> back;
	static const GenericVec3<T> xAxis;
	static const GenericVec3<T> yAxis;
	static const GenericVec3<T> zAxis;

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
GenericVec3<T>::GenericVec3(T all)
	: x(all)
	, y(all)
	, z(all)
{}

template<typename T>
GenericVec3<T>::GenericVec3(T x, T y, T z)
	: x(x)
	, y(y)
	, z(z)
{}

template<typename T>
template<typename U>
GenericVec3<T>::GenericVec3(const GenericVec3<U>& other)
	: x(static_cast<T>(other.x))
	, y(static_cast<T>(other.y))
	, z(static_cast<T>(other.z))
{}

template<typename T>
float GenericVec3<T>::length() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}

template<typename T>
float GenericVec3<T>::lengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

template<typename T>
GenericVec3<T> GenericVec3<T>::applied(T(*function)(T)) const
{
	return GenericVec3<T>(function(x), function(y), function(z));
}

template<typename T>
GenericVec3<T>& GenericVec3<T>::apply(T(*function)(T))
{
	*this = this->applied(function);
	return *this;
}

template<typename T>
GenericVec3<T> GenericVec3<T>::normalized() const
{
	T len = length();
	if (len == 0)
		return *this;
	return *this / length();
}

template<typename T>
void GenericVec3<T>::normalize()
{
	*this = normalized();
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
GenericVec3<T> GenericVec3<T>::operator*(const GenericVec3& rhs) const
{
	return GenericVec3(x * rhs.x, y * rhs.y, z * rhs.z);
}

template<typename T>
GenericVec3<T> GenericVec3<T>::operator/ (T rhs) const
{
	return GenericVec3(x / rhs, y / rhs, z / rhs);
}

template<typename T>
GenericVec3<T> GenericVec3<T>::operator/(const GenericVec3& rhs) const
{
	return GenericVec3(x / rhs.x, y / rhs.y, z / rhs.z);
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
GenericVec3<T> GenericVec3<T>::operator-() const
{
	return GenericVec3<T>(-x, -y, -z);
}

template<typename T>
const T& GenericVec3<T>::operator[](size_t i) const
{
	ASSERT(i < 3);
	return data()[i];
}

template<typename T>
T& GenericVec3<T>::operator[](size_t i)
{
	ASSERT(i < 3);
	return data()[i];
}

template<typename T>
GenericVec2<T> GenericVec3<T>::xz() const
{
	return GenericVec2<T>(x, z);
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

template<typename T>
float GenericVec3<T>::distance(const GenericVec3<T>& a, const GenericVec3<T>& b)
{
	return (a - b).length();
}

template<typename T>
float GenericVec3<T>::distanceSquared(const GenericVec3<T>& a, const GenericVec3<T>& b)
{
	return (a - b).lengthSquared();
}

template<typename T>
GenericVec3<T> GenericVec3<T>::randomInRange(const GenericVec3<T>& min, const GenericVec3<T>& max)
{
	static thread_local std::default_random_engine generator;
	return GenericVec3<T>(
		std::uniform_real_distribution<T>(min.x, max.x)(generator),
		std::uniform_real_distribution<T>(min.y, max.y)(generator),
		std::uniform_real_distribution<T>(min.z, max.z)(generator)
	);
}

// Right handed coorinate system
template <typename T>
const GenericVec3<T> GenericVec3<T>::up(0, 1, 0);

template <typename T>
const GenericVec3<T> GenericVec3<T>::down(0, -1, 0);

template <typename T>
const GenericVec3<T> GenericVec3<T>::left(1, 0, 0);

template <typename T>
const GenericVec3<T> GenericVec3<T>::right(-1, 0, 0);

template <typename T>
const GenericVec3<T> GenericVec3<T>::forward(0, 0, 1);

template <typename T>
const GenericVec3<T> GenericVec3<T>::back(0, 0, -1);

template <typename T>
const GenericVec3<T> GenericVec3<T>::xAxis(1, 0, 0);

template <typename T>
const GenericVec3<T> GenericVec3<T>::yAxis(0, 1, 0);

template <typename T>
const GenericVec3<T> GenericVec3<T>::zAxis(0, 0, 1);

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
