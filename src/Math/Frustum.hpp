#pragma once

#include <Math/Mat4.hpp>

struct AB
{
	Vec3 center;
	Vec3 extents;
};


struct Frustum
{
public:
	struct Plane
	{
		Vec3 normal;
		float distance;

		Plane() {};
		Plane(const Vec3& p1, const Vec3& norm)
			: normal(norm.normalized()),
			distance(dot(normal, p1))
		{}
	};
	//static Frustum fromViewPerspectiveMat(const Mat4& viewPerspectiveMat);

	Frustum() {};

public:
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;

public:
	static Frustum fromCamera(const Vec3& pos, const Vec3& forwardDir, float aspectRatio, float fov, float zNear, float zFar);
	//Vec3 

	bool collision(const AB& ab) const;
};