#pragma once

#include <Math/Vec3.hpp>

struct Plane
{
	Plane(const Vec3& normalizedNormal, float distanceFromOrigin);

	/*
	In a right handed cooridnate system the convention is to order the vertices counter clockwise 
	when looking at the front side (normal is facing at the viewer).
	*/	
	static Plane fromPoints(const Vec3& a, const Vec3& b, const Vec3& c);

	float signedDistance(const Vec3& p) const;
	bool isOnFrontSide(const Vec3& p) const;

	Vec3 normal;
	/*
	Signed distance from the origin. Negative means the origin is on the back side of the plane.
	*/
	float distance;
};