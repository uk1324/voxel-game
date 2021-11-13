#pragma once

#include <Math/Vec3.hpp>

#include <vector>

struct Mesh
{
public:
	Mesh() = default;

public:
	std::vector<Vec3> vertices;
};