#pragma once

#include <gtest/gtest.h>

#include <Math/Mat4.hpp>

TEST(Mat4Test, Scale)
{
	Mat4 m = Mat4::identity;
	Vec3 v(2, 3, 4);
	m = scale(m, v);
	EXPECT_EQ(m(0, 0), v.x);
	EXPECT_EQ(m(1, 1), v.y);
	EXPECT_EQ(m(2, 2), v.z);
	EXPECT_EQ(m(3, 3), 1);
}