#pragma once

#include <Engine/Ecs/Component.hpp>
#include <Math/Vec3.hpp>

struct Position : public Component
{
public:
	Position(Vec3 val);

public:
	Vec3 value;
};