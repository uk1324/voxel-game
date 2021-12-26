#pragma once

#include <Math/Quat.hpp>

class Rotation
{
public:
	Rotation(const Quat& val);

public:
	Quat value;
};