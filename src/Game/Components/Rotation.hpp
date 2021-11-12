#pragma once

#include <Engine/Ecs/Component.hpp>
#include <Math/Quaternion.hpp>

class Rotation : public Component
{
public:
	Rotation(const Quaternion& val);

public:
	Quaternion value;
};