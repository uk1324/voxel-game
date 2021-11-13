#pragma once

#include <Engine/Ecs/Component.hpp>
#include <Engine/Graphics/Mesh.hpp>

struct MeshRenderer : public Component
{
	Mesh* mesh;
};