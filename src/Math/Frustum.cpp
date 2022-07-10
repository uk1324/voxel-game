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
