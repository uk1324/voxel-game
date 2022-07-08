#pragma once

#include <Math/Vec3.hpp>
#include <Utils/Assertions.hpp>

#include <ostream>

// Columnwise matrix
// Multiplication is reversed so a * b means transform a by b and to the opposite like it would be mathematically
// when colum matrices are used.
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
	GenericMat4<T> operator* (const GenericMat4<T> mat) const;
	GenericMat4<T>& operator*= (const GenericMat4<T> mat);

	GenericMat4<T> inverse() const;
	GenericMat4<T> transpose() const;

	GenericMat4<T> removedTranslation() const;
	void removeTranslation();
	void setTranslation(const Vec3& translation);

	T* data();
	const T* data() const;

public:
	static GenericMat4<T> scale(const Vec3& scale);
	static GenericMat4<T> translation(const Vec3& translation);
	static GenericMat4<T> perspective(T fov, T aspectRatio, T nearZ, T farZ);
	static GenericMat4<T> orthographic(GenericVec3<T> min, GenericVec3<T> max);
	static GenericMat4<T> lookAt(GenericVec3<T> pos, GenericVec3<T> target, GenericVec3<T> up);

public:
	static const GenericMat4<T> identity;

public:
	static constexpr size_t WIDTH = 4;
	static constexpr size_t HEIGHT = 4;

private:
	T m_data[16];
};

template<typename T>
std::ostream& operator<< (std::ostream& os, GenericMat4<T> mat);

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
GenericMat4<T> GenericMat4<T>::operator* (const GenericMat4<T> mat) const
{
	GenericMat4<T> m;
	for (size_t c = 0; c < WIDTH; c++) {
		for (size_t r = 0; r < HEIGHT; r++) {
			m(r, c) = get(r, 0) * mat(0, c)
					+ get(r, 1) * mat(1, c)
					+ get(r, 2) * mat(2, c)
					+ get(r, 3) * mat(3, c);
		}
	}
	//return m;
	//m(0, 0) = get(0, 0) * mat.get(0, 0) + get(0, 1) * mat.get(1, 0) + get(0, 2) * mat.get(2, 0) + get(0, 3) * mat.get(3, 0);
	//m(0, 1) = get(0, 0) * mat.get(0, 1) + get(0, 1) * mat.get(1, 1) + get(0, 2) * mat.get(2, 1) + get(0, 3) * mat.get(3, 1);
	//m(0, 2) = get(0, 0) * mat.get(0, 2) + get(0, 1) * mat.get(1, 2) + get(0, 2) * mat.get(2, 2) + get(0, 3) * mat.get(3, 2);
	//m(0, 3) = get(0, 0) * mat.get(0, 3) + get(0, 1) * mat.get(1, 3) + get(0, 2) * mat.get(2, 3) + get(0, 3) * mat.get(3, 3);
	//m(1, 0) = get(1, 0) * mat.get(0, 0) + get(1, 1) * mat.get(1, 0) + get(1, 2) * mat.get(2, 0) + get(1, 3) * mat.get(3, 0);
	//m(1, 1) = get(1, 0) * mat.get(0, 1) + get(1, 1) * mat.get(1, 1) + get(1, 2) * mat.get(2, 1) + get(1, 3) * mat.get(3, 1);
	//m(1, 2) = get(1, 0) * mat.get(0, 2) + get(1, 1) * mat.get(1, 2) + get(1, 2) * mat.get(2, 2) + get(1, 3) * mat.get(3, 2);
	//m(1, 3) = get(1, 0) * mat.get(0, 3) + get(1, 1) * mat.get(1, 3) + get(1, 2) * mat.get(2, 3) + get(1, 3) * mat.get(3, 3);
	//m(2, 0) = get(2, 0) * mat.get(0, 0) + get(2, 1) * mat.get(1, 0) + get(2, 2) * mat.get(2, 0) + get(2, 3) * mat.get(3, 0);
	//m(2, 1) = get(2, 0) * mat.get(0, 1) + get(2, 1) * mat.get(1, 1) + get(2, 2) * mat.get(2, 1) + get(2, 3) * mat.get(3, 1);
	//m(2, 2) = get(2, 0) * mat.get(0, 2) + get(2, 1) * mat.get(1, 2) + get(2, 2) * mat.get(2, 2) + get(2, 3) * mat.get(3, 2);
	//m(2, 3) = get(2, 0) * mat.get(0, 3) + get(2, 1) * mat.get(1, 3) + get(2, 2) * mat.get(2, 3) + get(2, 3) * mat.get(3, 3);
	//m(3, 0) = get(3, 0) * mat.get(0, 0) + get(3, 1) * mat.get(1, 0) + get(3, 2) * mat.get(2, 0) + get(3, 3) * mat.get(3, 0);
	//m(3, 1) = get(3, 0) * mat.get(0, 1) + get(3, 1) * mat.get(1, 1) + get(3, 2) * mat.get(2, 1) + get(3, 3) * mat.get(3, 1);
	//m(3, 2) = get(3, 0) * mat.get(0, 2) + get(3, 1) * mat.get(1, 2) + get(3, 2) * mat.get(2, 2) + get(3, 3) * mat.get(3, 2);
	//m(3, 3) = get(3, 0) * mat.get(0, 3) + get(3, 1) * mat.get(1, 3) + get(3, 2) * mat.get(2, 3) + get(3, 3) * mat.get(3, 3);


	//for (int i = 0; i < 4; i++) {
	//	for (int j = 0; j < 4; j++) {
	//		m.set(i, j, 0);

	//		for (int k = 0; k < 4; k++) {
	//			m(i, j) += get(i, k) * mat.get(k, j);
	//		}

	//	}
	//}

	return m;

}

template<typename T>
GenericMat4<T>& GenericMat4<T>::operator*= (const GenericMat4<T> mat)
{
	*this = *this * mat;
	return *this;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::inverse() const
{
	T inv[16], det;
	int i;

	const T* m = data();

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	GenericMat4<T> inverse;

	if (det == 0)
		return inverse;

	det = 1.0 / det;


	for (i = 0; i < 16; i++)
		inverse.data()[i] = inv[i] * det;

	return inverse;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::transpose() const
{
	GenericMat4<T> mat;

	mat(0, 0) = get(1, 1);
	mat(1, 0) = get(0, 1);
	mat(2, 0) = get(0, 2);
	mat(3, 0) = get(0, 3);

	mat(0, 1) = get(1, 0);
	mat(1, 1) = get(1, 1);
	mat(2, 1) = get(1, 2);
	mat(3, 1) = get(1, 3);

	mat(0, 2) = get(2, 0);
	mat(1, 2) = get(2, 1);
	mat(2, 2) = get(2, 2);
	mat(3, 2) = get(2, 3);

	mat(0, 3) = get(3, 0);
	mat(1, 3) = get(3, 1);
	mat(2, 3) = get(3, 2);
	mat(3, 3) = get(3, 3);

	return mat;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::removedTranslation() const
{
	GenericMat4<T> mat = *this;
	mat.removeTranslation();
	return mat;
}

template<typename T>
void GenericMat4<T>::removeTranslation()
{
	set(3, 0, 0);
	set(3, 1, 0);
	set(3, 2, 0);
}

template<typename T>
void GenericMat4<T>::setTranslation(const Vec3& translation)
{
	set(3, 0, translation.x);
	set(3, 1, translation.y);
	set(3, 2, translation.z);
}

template<typename T>
GenericMat4<T> GenericMat4<T>::scale(const Vec3& scale)
{
	Mat4 mat;

	mat.set(0, 0, scale.x);
	mat.set(1, 1, scale.y);
	mat.set(2, 2, scale.z);
	mat.set(3, 3, 1);

	//operator()(0, 0) *= scale.x;
	//operator()(1, 0) *= scale.x;
	//operator()(2, 0) *= scale.x;
	//operator()(3, 0) *= scale.x;
	//
	//operator()(0, 1) *= scale.y;
	//operator()(1, 1) *= scale.y;
	//operator()(2, 1) *= scale.y;
	//operator()(3, 1) *= scale.y;
	//
	//operator()(0, 2) *= scale.z;
	//operator()(1, 2) *= scale.z;
	//operator()(2, 2) *= scale.z;
	//operator()(3, 2) *= scale.z;
	return mat;
}

template<typename T>
T* GenericMat4<T>::data()
{
	return m_data;
}

template<typename T>
const T* GenericMat4<T>::data() const
{
	return m_data;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::translation(const Vec3& translation)
{
	Mat4 m = m.identity;
	m(3, 0) = translation.x;
	m(3, 1) = translation.y;
	m(3, 2) = translation.z;
	return m;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::perspective(T fov, T aspectRatio, T nearZ, T farZ)
{
	// near and far represent the z position of the clip plane

	// The frustrum is defined by the aspect ratio of the near plane and the fov

	// Maps the points inside the frustrum to the cube from (-1, -1, -1) to (1, 1, 1), because
	// OpenGL clips all of the triangles outside of the cube.
	T fovInv = 1.0f / tanf(fov * 0.5f);
	GenericMat4<T> mat;
	//mat(0, 0) = fovInv;
	//mat(1, 1) = aspectRatio * fovInv;
	//mat(2, 2) = far / (far - near);
	//mat(3, 2) = (-far * near) / (far - near);
	//mat(2, 3) = 1.0f;

	mat(0, 0) = T(1) / (aspectRatio * fovInv);
	mat(1, 1) = T(1) / (fovInv);
	mat(2, 2) = -(farZ + nearZ) / (farZ - nearZ);
	mat(2, 3) = -T(1);
	mat(3, 2) = -(T(2) * farZ * nearZ) / (farZ - nearZ);

	//T fovInv = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);
	//Mat4 mat;
	//mat(0, 0) = fovInv / aspectRatio;
	//mat(1, 1) = fov;
	//mat(2, 2) = (far + near) / (near - far);
	//mat(3, 2) = (2 * far * near) / (near - far);
	//mat(2, 3) = 1.0f;


	return mat;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::orthographic(GenericVec3<T> min, GenericVec3<T> max)
{
	GenericMat4<T> mat;

	// Squish to AABB(min = [-1, -1, -1], max = [1, 1, 1]).
	mat(0, 0) = T(2) / (max.x - min.x);
	mat(1, 1) = T(2) / (max.y - min.y);
	mat(2, 2) = T(-2) / (max.z - min.z);
	// Translate to center.
	mat(3, 0) = -(max.x + min.x) / (max.x - min.x);
	mat(3, 1) = -(max.y + min.y) / (max.y - min.y);
	mat(3, 2) = -(max.z + min.z) / (max.z - min.z);

	mat(3, 3) = T(1);

	return mat;
}

template<typename T>
GenericMat4<T> GenericMat4<T>::lookAt(GenericVec3<T> position, GenericVec3<T> target, GenericVec3<T> up)
{
	// An orthogonal matrix is a matrix with all the basis orthogonal (perpendicular) to each other.
	// An orthonormal matrix is an orthogonal matrix with basis of length 1.

	GenericVec3<T> lookDirection = target - position;

	// Calculate an orthonormal basis using the Gram-Schmidt process.

	GenericVec3<T> forward = lookDirection.normalized();
	// Forward and up create a plane so the cross product is the right vector.
	GenericVec3<T> right = (cross(forward, up)).normalized();
	GenericVec3<T> projectedUp(cross(right, forward));

	Mat4 m;
	// The inverse of a orthogonal matrix is equal to it's transpose so to transpose it I put the transformed basis into rows
	// and not columns. 
	m(0, 0) = right.x;
	m(1, 0) = right.y;
	m(2, 0) = right.z;

	m(0, 1) = projectedUp.x;
	m(1, 1) = projectedUp.y;
	m(2, 1) = projectedUp.z;

	// Don't know why, but it works if it is negated.
	m(0, 2) = -forward.x;
	m(1, 2) = -forward.y;
	m(2, 2) = -forward.z;

	m(3, 3) = 1;

	// One problem with the Gram-Schmidt process is that if fails when the forward vector and the up vector are
	// linearly depended (they lie on the same line) because the cross product of up and forward is equal to the zero vector.

	//position.y = -position.y;
	return GenericMat4<T>::translation(-position) * m;
}

template<typename T>
const GenericMat4<T> GenericMat4<T>::identity = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

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