#include <Math/Frustum.hpp>

static bool isOnOrForwardPlan(const AB& a, const const Frustum::Plane& plan)
{
    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    const float r = a.extents.x * std::abs(plan.normal.x) +
        a.extents.y * std::abs(plan.normal.y) + a.extents.z * std::abs(plan.normal.z);

    return -r <= dot(plan.normal, a.center) - plan.distance;
}


Frustum Frustum::fromCamera(const Vec3& pos, const Vec3& forwardDir, float aspectRatio, float fov, float zNear, float zFar)
{
    const Vec3 rightDir = cross(forwardDir, Vec3::up);
    const Vec3 upDir = cross(rightDir, forwardDir);

    Frustum     frustum;
    const float halfVSide = zFar * tanf(fov * .5f);
    const float halfHSide = halfVSide * aspectRatio;
    const Vec3 frontMultFar = forwardDir * zFar;

    frustum.nearFace = { pos + forwardDir * zNear, forwardDir };
    frustum.farFace = { pos + frontMultFar, -forwardDir };
    frustum.rightFace = { pos,
                            cross(upDir ,frontMultFar + rightDir * halfHSide) };
    frustum.leftFace = { pos,
                            cross(frontMultFar - rightDir * halfHSide, upDir) };
    frustum.topFace = { pos,
                            cross(rightDir, frontMultFar - upDir * halfVSide) };
    frustum.bottomFace = { pos,
                            cross(frontMultFar + upDir * halfVSide, rightDir) };
    return frustum;
}

bool Frustum::collision(const AB& ab) const
{
    return isOnOrForwardPlan(ab, leftFace) &&
        isOnOrForwardPlan(ab, rightFace) &&
        isOnOrForwardPlan(ab, topFace) &&
        isOnOrForwardPlan(ab, bottomFace) &&
        isOnOrForwardPlan(ab, nearFace) &&
        isOnOrForwardPlan(ab, farFace);
}
