#pragma once

#include <Math/Mat4.hpp>
#include <Math/Aabb.hpp>
#include <Math/Plane.hpp>

struct Frustum
{
public:
	Frustum(const Mat4& toNdc);

private:
	Plane m_front;
	Plane m_back;
	Plane m_left;
	Plane m_right;
	Plane m_top;
	Plane m_bottom;

public:

	bool intersects(const Aabb& aabb) const;
};