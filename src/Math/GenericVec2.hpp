#pragma once

#include <ostream>

template<typename T>
struct GenericVec2
{
public:
	GenericVec2();
	explicit GenericVec2(T all);
	template<typename U>
	explicit GenericVec2(const GenericVec2<U>& other);
	GenericVec2(T x, T y);
	float length();

	bool operator== (const GenericVec2& rhs) const;
	bool operator!= (const GenericVec2& rhs) const;
	GenericVec2 operator+(const GenericVec2& rhs) const;
	GenericVec2& operator+=(const GenericVec2& rhs);
	GenericVec2 operator-(const GenericVec2& rhs) const;
	GenericVec2& operator-=(const GenericVec2& rhs);
	GenericVec2 operator*(T rhs) const;
	GenericVec2& operator*=(T rhs);
	GenericVec2 operator*(const GenericVec2<T>& rhs) const;
	GenericVec2 operator/(T rhs) const;
	GenericVec2& operator/=(T rhs);
	GenericVec2 operator/(const GenericVec2& rhs) const;
	GenericVec2& operator/=(const GenericVec2& rhs);
	GenericVec2 operator-() const;

	GenericVec2 negatedX() const;
	GenericVec2 negatedY() const;

	T* data();
	const T* data() const;

public:
	T x;
	T y;
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const GenericVec2<T>& vec);

template<typename T>
T dot(GenericVec2<T> lhs, GenericVec2<T> rhs);

template<typename T>
T cross(GenericVec2<T> lhs, GenericVec2<T> rhs);

template<typename T>
GenericVec2<T>::GenericVec2()
	: x(0)
	, y(0)
{}

template<typename T>
GenericVec2<T>::GenericVec2(T all)
	: x(all)
	, y(all)
{}

template<typename T>
template<typename U>
GenericVec2<T>::GenericVec2(const GenericVec2<U>& other)
	: x(static_cast<T>(other.x))
	, y(static_cast<T>(other.y))
{}

template<typename T>
GenericVec2<T>::GenericVec2(T x, T y)
	: x(x)
	, y(y)
{}

template<typename T>
float GenericVec2<T>::length()
{
	return sqrt((x * x) + (y * y));
}

template<typename T>
bool GenericVec2<T>::operator== (const GenericVec2& rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}

template<typename T>
bool GenericVec2<T>::operator!= (const GenericVec2& rhs) const
{
	return (x != rhs.x) || (y != rhs.y);
}

template<typename T>
GenericVec2<T> GenericVec2<T>::operator+ (const GenericVec2& rhs) const
{
	return GenericVec2(x + rhs.x, y + rhs.y);
}

template<typename T>
GenericVec2<T>& GenericVec2<T>::operator+= (const GenericVec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

template<typename T>
GenericVec2<T> GenericVec2<T>::operator- (const GenericVec2& rhs) const
{
	return GenericVec2(x - rhs.x, y - rhs.y);
}

template<typename T>
GenericVec2<T>& GenericVec2<T>::operator-= (const GenericVec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

template<typename T>
GenericVec2<T> GenericVec2<T>::operator* (T rhs) const
{
	return GenericVec2(x * rhs, y * rhs);
}

template<typename T>
GenericVec2<T>& GenericVec2<T>::operator*= (T rhs)
{
	x *= rhs;
	y *= rhs;
	return *this;
}

template<typename T>
GenericVec2<T> GenericVec2<T>::operator*(const GenericVec2<T>& rhs) const
{
	return GenericVec2(x * rhs.x, y * rhs.y);
}

template<typename T>
GenericVec2<T> GenericVec2<T>::operator/ (T rhs) const
{
	return GenericVec2(x / rhs, y / rhs);
}

template<typename T>
GenericVec2<T>& GenericVec2<T>::operator/= (T rhs)
{
	x /= rhs;
	y /= rhs;
	return *this;
}

template<typename T>
GenericVec2<T> GenericVec2<T>::operator/ (const GenericVec2& rhs) const
{
	return GenericVec2(x / rhs.x, y / rhs.y);
}

template<typename T>
GenericVec2<T>& GenericVec2<T>::operator/= (const GenericVec2& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
	return *this;
}

template<typename T>
GenericVec2<T> GenericVec2<T>::operator-() const
{
	return GenericVec2<T>(-x, -y);
}

template<typename T>
GenericVec2<T> GenericVec2<T>::negatedX() const
{
	return GenericVec2<T>(-x, y);
}

template<typename T>
GenericVec2<T> GenericVec2<T>::negatedY() const
{
	return GenericVec2<T>(x, -y);
}

template<typename T>
T* GenericVec2<T>::data()
{
	return &x;
}

template<typename T>
const T* GenericVec2<T>::data() const
{
	return &x;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const GenericVec2<T>& vec)
{
	os << '(' << vec.x << ", " << vec.y << ')';
	return os;
}

template<typename T>
T dot(GenericVec2<T> lhs, GenericVec2<T> rhs)
{
	return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

template<typename T>
T cross(GenericVec2<T> lhs, GenericVec2<T> rhs)
{
	return (lhs.x * rhs.y) - (rhs.x * lhs.y);
}