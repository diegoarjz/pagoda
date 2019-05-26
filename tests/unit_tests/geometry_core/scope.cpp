#include <geometry_core/scope.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(Scope, test_scope_construction)
{
	Scope s;

	EXPECT_EQ(s.GetPosition(), Vec3F(0, 0, 0));
	EXPECT_EQ(s.GetSize(), Vec3F(0, 0, 0));
	EXPECT_EQ(s.GetRotation().Row(0), Vec3F(1, 0, 0));
	EXPECT_EQ(s.GetRotation().Row(1), Vec3F(0, 1, 0));
	EXPECT_EQ(s.GetRotation().Row(2), Vec3F(0, 0, 1));
}
