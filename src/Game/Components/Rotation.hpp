#pragma once

#include <Math/Quaternion.hpp>

class Rotation
{
public:
	Rotation(const Quat& val);

public:
	Quat value;
};