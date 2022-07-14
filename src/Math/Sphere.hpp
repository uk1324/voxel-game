#pragma once

#include <Math/Vec3.hpp>

struct Sphere
{
	Sphere(const Vec3& center, float radius);

	Vec3 center;
	float radius;
};