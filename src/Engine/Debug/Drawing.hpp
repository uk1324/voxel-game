#pragma once

#include <Math/Box.hpp>

// Don't know if this should be a class or a namespace
namespace Debug
{
	void drawLine(const Vec3& start, const Vec3& end, const Vec3& color);
	void drawBox(const Box& box);
}