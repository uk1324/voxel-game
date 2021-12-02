#include <Engine/Ecs/Entity.hpp>
#include <Math/Mat4.hpp>

// Maybe make a event system to translate GLFW events to my events.
// It would also be nice to find a library that lets me poll events like in SDL

class CameraSystem
{
public:
	CameraSystem(float screenWidth, float screenHeight, float fov, float nearPlaneZ, float farPlaneZ);
	void update(const Entity& player, float screenWidth, float screenHeight);

	const Mat4 projection();
	const Mat4& view();

public:
	float screenWidth;
	float screenHeight;
	float fov;
	float nearPlaneZ;
	float farPlaneZ;

private:
	Mat4 m_view;
};