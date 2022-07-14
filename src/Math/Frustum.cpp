#include <Math/Frustum.hpp>
#include <array>

Frustum::Frustum(const Mat4& toNdc)
	:m_front(Vec3(0), 0)
	, m_back(Vec3(0), 0)
	, m_left(Vec3(0), 0)
	, m_right(Vec3(0), 0)
	, m_top(Vec3(0), 0)
	, m_bottom(Vec3(0), 0)
{
	const auto inverse = toNdc.inverse();

	enum Corners
	{
		BackTopRight,
		BackBottomRight,
		BackTopLeft,
		BackBottomLeft,
		FrontTopRight,
		FrontBottomRight,
		FrontTopLeft,
		FrontBottomLeft,
	};

	const std::array<Vec3, 8> corners = {
		inverse * Vec3(1, 1, 1),
		inverse * Vec3(1, -1, 1),
		inverse * Vec3(-1, 1, 1),
		inverse * Vec3(-1, -1, 1),
		inverse * Vec3(1, 1, -1),
		inverse * Vec3(1, -1, -1),
		inverse * Vec3(-1, 1, -1),
		inverse * Vec3(-1, -1, -1),
	};

	m_front = Plane::fromPoints(corners[FrontBottomLeft], corners[FrontBottomRight], corners[FrontTopRight]);
	m_back = Plane::fromPoints(corners[BackTopRight], corners[BackBottomRight], corners[BackBottomLeft]);

	m_bottom = Plane::fromPoints(corners[BackBottomRight], corners[FrontBottomRight], corners[FrontBottomLeft]);
	m_top = Plane::fromPoints(corners[FrontTopLeft], corners[FrontTopRight], corners[BackTopRight]);

	m_left = Plane::fromPoints(corners[BackBottomLeft], corners[FrontBottomLeft], corners[FrontTopLeft]);
	m_right = Plane::fromPoints(corners[FrontTopRight], corners[FrontBottomRight], corners[BackBottomRight]);
}

bool Frustum::intersects(const Aabb& aabb) const
{
	auto anyPointsInside = [&aabb](const Plane& plane) -> bool
	{
		const auto size = aabb.max - aabb.min;
		return !plane.isOnFrontSide(aabb.min)
			|| !plane.isOnFrontSide(aabb.max)
			|| !plane.isOnFrontSide(aabb.min + Vec3(size.x, 0, 0))
			|| !plane.isOnFrontSide(aabb.min + Vec3(0, size.y, 0))
			|| !plane.isOnFrontSide(aabb.min + Vec3(0, 0, size.z))
			|| !plane.isOnFrontSide(aabb.min + Vec3(0, size.y, size.z))
			|| !plane.isOnFrontSide(aabb.min + Vec3(size.x, 0, size.z))
			|| !plane.isOnFrontSide(aabb.min + Vec3(size.x, size.y, 0));
	};

	return anyPointsInside(m_front)
		&& anyPointsInside(m_back)
		&& anyPointsInside(m_bottom)
		&& anyPointsInside(m_top)
		&& anyPointsInside(m_left)
		&& anyPointsInside(m_right);
}

bool Frustum::intersects(const Sphere& sphere) const
{
	//return intersects(Aabb(sphere.center - Vec3(sphere.radius), sphere.center + Vec3(sphere.radius)));

	return (m_front.signedDistance(sphere.center) < sphere.radius)
		&& (m_back.signedDistance(sphere.center) < sphere.radius)
		&& (m_bottom.signedDistance(sphere.center) < sphere.radius)
		&& (m_top.signedDistance(sphere.center) < sphere.radius)
		&& (m_left.signedDistance(sphere.center) < sphere.radius)
		&& (m_right.signedDistance(sphere.center) < sphere.radius);

	// This one also works
	/*const auto distanceFront = m_front.signedDistance(sphere.center);
	const auto distanceBack = m_back.signedDistance(sphere.center);
	const auto distanceBottom = m_bottom.signedDistance(sphere.center);
	const auto distanceTop = m_top.signedDistance(sphere.center);
	const auto distanceLeft = m_left.signedDistance(sphere.center);
	const auto distanceRight = m_right.signedDistance(sphere.center);

	return (distanceLeft < 0 && distanceRight < 0) || abs(distanceLeft) <= sphere.radius || abs(distanceRight) <= sphere.radius
		&& (distanceBack < 0 && distanceFront < 0) || abs(distanceFront) <= sphere.radius || abs(distanceBack) <= sphere.radius
		&& (distanceTop < 0 && distanceBottom < 0) || abs(distanceTop) <= sphere.radius || abs(distanceBottom) <= sphere.radius;*/
}
