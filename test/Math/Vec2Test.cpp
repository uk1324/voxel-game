#include <gtest/gtest.h>

#include <Math/Vec2.hpp>

TEST(Vec2Test, Addition)
{
	EXPECT_EQ(Vec2(1, 2) + Vec2(3, 4), Vec2(4, 6));

	Vec2 v(1, 2);
	v += Vec2(3, 4);
	EXPECT_EQ(v, Vec2(4, 6));
}

TEST(Vec2Test, Subtraction)
{
	EXPECT_EQ(Vec2(1, 2) - Vec2(3, 4), Vec2(-2, -2));

	Vec2 v(1, 2);
	v -= Vec2(3, 4);
	EXPECT_EQ(v, Vec2(-2, -2));
}

TEST(Vec2Test, ScalarMultiplication)
{
	EXPECT_EQ(Vec2(1, 2) * 3, Vec2(3, 6));

	Vec2 v(1, 2);
	v *= 3;
	EXPECT_EQ(v, Vec2(3, 6));
}

TEST(Vec2Test, ScalarDivision)
{
	EXPECT_EQ(Vec2(9, 6) / 3, Vec2(3, 2));

	Vec2 v(9, 6);
	v /= 3;
	EXPECT_EQ(v, Vec2(3, 2));
}

TEST(Vec2Test, Comparasion)
{
	EXPECT_EQ(Vec2(1, 2), Vec2(1, 2));
	EXPECT_NE(Vec2(1, 3), Vec2(1, 2));
}

TEST(Vec2Test, Length)
{
	EXPECT_EQ(Vec2(5, 0).length(), 5);
	EXPECT_EQ(Vec2(3, 4).length(), 5);
}

TEST(Vec2Test, DotProduct)
{
	EXPECT_EQ(dot(Vec2(1, 0), Vec2(0, 2)), 0);
	EXPECT_EQ(dot(Vec2(3, 0), Vec2(2, 0)), 6);
}

TEST(Vec2Test, CrossProduct)
{
	EXPECT_EQ(cross(Vec2(1, 0), Vec2(0, 2)), 2);
	EXPECT_EQ(cross(Vec2(3, 0), Vec2(2, 0)), 0);
}