#pragma once

#include <Math/Vec3.hpp>
#include <Utils/Assertions.hpp>

#include <ostream>

// Column wise access
template<typename T>
class GenericMat4
{
public:
	GenericMat4();
	GenericMat4(std::initializer_list<T> mat);

	void set(size_t x, size_t y, T value);
	const T get(size_t x, size_t y) const;
	T& operator()(size_t x, size_t y);
	const T operator()(size_t x, size_t y) const;

	GenericVec3<T> operator* (const GenericVec3<T>& vec) const;
	GenericMat4<T> operator* (T scalar) const;

	T* data();

public:
	static GenericMat4<T> identity();

public:
	static constexpr size_t WIDTH = 4;
	static constexpr size_t HEIGHT = 4;

private:
	T m_data[16];
};

template<typename T>
std::ostream& operator<< (std::ostream& os, GenericMat4<T> mat);

// Translates the matrix by adding vec to the w vector.
template<typename T>
GenericMat4<T> translate(const GenericMat4<T>& mat, const GenericVec3<T>& vec);
// Scales the basis vectors by components of vec.
template<typename T>
GenericMat4<T> scale(const GenericMat4<T>& mat, const GenericVec3<T>& vec);

template<typename T>
GenericMat4<T>::GenericMat4()
{
	for (size_t x = 0; x < WIDTH; x++)
	{
		for (size_t y = 0; y < HEIGHT; y++)
		{
			set(x, y, 0);
		}
	}
}

template<typename T>
GenericMat4<T>::GenericMat4(std::initializer_list<T> mat)
{
	ASSERT(mat.size() == (WIDTH * HEIGHT));
	for (size_t y = 0; y < GenericMat4<T>::HEIGHT; y++)
	{
		for (size_t x = 0; x < GenericMat4<T>::WIDTH; x++)
		{
			set(x, y, mat.begin()[y * WIDTH + x]);
		}
	}
}

template<typename T>
void GenericMat4<T>::set(size_t x, size_t y, T value)
{
	this->operator()(x, y) = value;
}

template<typename T>
const T GenericMat4<T>::get(size_t x, size_t y) const
{
	return this->operator()(x, y);
}

template<typename T>
T& GenericMat4<T>::operator()(size_t x, size_t y)
{
	ASSERT((x < WIDTH) && (y < HEIGHT));
	return m_data[x * HEIGHT + y];
}

template<typename T>
const T GenericMat4<T>::operator()(size_t x, size_t y) const
{
	return m_data[x * HEIGHT + y];
}

template<typename T>
GenericVec3<T> GenericMat4<T>::operator*(const GenericVec3<T>& vec) const
{
	GenericVec3<T> v(
		vec.x * get(0, 0) + vec.y * get(1, 0) + vec.z * get(2, 0) + get(3, 0),
		vec.x * get(0, 1) + vec.y * get(1, 1) + vec.z * get(2, 1) + get(3, 1),
		vec.x * get(0, 2) + vec.y * get(1, 2) + vec.z * get(2, 2) + get(3, 2)
	);
	T w = vec.x * get(0, 3) + vec.y * get(1, 3) + vec.z * get(2, 3) + get(3, 3);
	v /= w;

	return v;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::operator*(T scalar) const
{
	Mat4 m = *this;
	for (size_t x = 0; x < WIDTH; x++)
	{
		for (size_t y = 0; y < HEIGHT; y++)
		{
			m(x, y) = get(x, y) * scalar;
		}
	}
	return m;
}

template<typename T>
T* GenericMat4<T>::data()
{
	return m_data;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::identity()
{
	Mat4 mat;
	mat(0, 0) = 1;
	mat(1, 1) = 1;
	mat(2, 2) = 1;
	mat(3, 3) = 1;
	return mat;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, GenericMat4<T> mat)
{
	for (size_t y = 0; y < GenericMat4<T>::HEIGHT; y++)
	{ 
		for (size_t x = 0; x < GenericMat4<T>::WIDTH; x++)
		{
			os << mat.get(x, y) << ' ';
		}
		os << "\n";
	}

	return os;
}

template<typename T>
GenericMat4<T> translate(const GenericMat4<T>& mat, const GenericVec3<T>& vec)
{
	GenericMat4<T> m = mat;
	m(3, 0) += vec.x;
	m(3, 1) += vec.y;
	m(3, 2) += vec.z;
	return m;
}

template<typename T>
GenericMat4<T> scale(const GenericMat4<T>& mat, const GenericVec3<T>& vec)
{
	GenericMat4<T> m = mat;
	m(0, 0) *= vec.x;
	m(1, 0) *= vec.x;
	m(2, 0) *= vec.x;
	m(3, 0) *= vec.x;

	m(0, 1) *= vec.y;
	m(1, 1) *= vec.y;
	m(2, 1) *= vec.y;
	m(3, 1) *= vec.y;

	m(0, 2) *= vec.z;
	m(1, 2) *= vec.z;
	m(2, 2) *= vec.z;
	m(3, 2) *= vec.z;

	return m;
}
