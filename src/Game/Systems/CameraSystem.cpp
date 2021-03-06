#include <Game/Systems/CameraSystem.hpp>
#include <Game/Components/Position.hpp>
#include <Game/Components/Rotation.hpp>

CameraSystem::CameraSystem(float fov, float nearPlaneZ, float farPlaneZ)
	: fov(fov)
	, nearPlaneZ(nearPlaneZ)
	, farPlaneZ(farPlaneZ)
{}

// Don't know if it is a good idea but the view matrix is generated in update.
void CameraSystem::update(const EntityManager& entityManager, const Entity& player, float screenWidth, float screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	const Vec3& playerPos = entityManager.getComponent<Position>(player).value;
	const Quat& playerRotation = entityManager.getComponent<Rotation>(player).value;

	Vec3 direction = (playerRotation * Vec3::forward).normalized();
	Vec3 target = playerPos + direction;

	m_view = Mat4::lookAt(playerPos, target, Vec3::up);
}

const Mat4 CameraSystem::projection()
{
	return Mat4::perspective(fov, screenWidth / screenHeight, nearPlaneZ, farPlaneZ);
}

const Mat4& CameraSystem::view()
{
	return m_view;
}
