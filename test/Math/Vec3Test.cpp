#include <gtest/gtest.h>

#include <Math/Vec3.hpp>

TEST(Vec3Test, Addition)
{
	EXPECT_EQ(Vec3(1, 2, 3) + Vec3(3, 4, 2), Vec3(4, 6, 5));

	Vec3 v(1, 2, 3);
	v += Vec3(3, 4, 2);
	EXPECT_EQ(v, Vec3(4, 6, 5));
}

TEST(Vec3Test, Subtraction)
{
	EXPECT_EQ(Vec3(1, 2, 3) - Vec3(3, 4, 2), Vec3(-2, -2, 1));

	Vec3 v(1, 2, 3);
	v -= Vec3(3, 4, 2);
	EXPECT_EQ(v, Vec3(-2, -2, 1));
}

TEST(Vec3Test, ScalarMultiplication)
{
	EXPECT_EQ(Vec3(1, 2, 5) * 3, Vec3(3, 6, 15));

	Vec3 v(1, 2, 5);
	v *= 3;
	EXPECT_EQ(v, Vec3(3, 6, 15));
}

TEST(Vec3Test, ScalarDivision)
{
	EXPECT_EQ(Vec3(6, 8, 12) / 2, Vec3(3, 4, 6));

	Vec3 v(6, 8, 12);
	v /= 2;
	EXPECT_EQ(v, Vec3(3, 4, 6));
}

TEST(Vec3Test, Comparasion)
{
	EXPECT_EQ(Vec3(1, 2, 3), Vec3(1, 2, 3));
	EXPECT_NE(Vec3(1, 3, 2), Vec3(1, 2, 3));
}

TEST(Vec3Test, Length)
{
	EXPECT_EQ(Vec3(5, 0, 0).length(), 5);
	EXPECT_EQ(Vec3(1, 2, 2).length(), 3);
}

TEST(Vec3Test, DotProduct)
{
	EXPECT_EQ(dot(Vec3(1, 0, 0), Vec3(0, 2, 0)), 0);
	EXPECT_EQ(dot(Vec3(3, 0, 0), Vec3(2, 0, 0)), 6);
}

TEST(Vec3Test, CrossProduct)
{
	EXPECT_EQ(cross(Vec3(2, 0, 0), Vec3(0, 3, 0)), Vec3(0, 0, 6));
}