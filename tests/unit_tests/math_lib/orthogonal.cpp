#include <pagoda/math_lib/vec_base.h>

#include <pagoda/math_lib/orthogonal.h>

#include <boost/qvm/vec_operations.hpp>

#include <gtest/gtest.h>

using namespace pagoda;

TEST(Orthogonal, when_calculating_the_orthogonal_vector_of_a_2d_vector_should_have_zero_as_dot_product)
{
	Vec2F v{1, 2};
	ASSERT_EQ(boost::qvm::dot(v, orthogonal(v)), 0);
}

TEST(Orthogonal, when_calculating_the_orthogonal_vector_of_a_3d_vector_should_have_zero_as_dot_product)
{
	Vec3F v{1, 2, 3};
	ASSERT_EQ(boost::qvm::dot(v, orthogonal(v)), 0);
}

TEST(Orthogonal, when_calculating_the_orthogonal_of_a_vector_parallel_with_an_axis_should_have_zero_as_dot_product)
{
	for (auto v : {Vec3F{1, 0, 0}, Vec3F{0, 1, 0}, Vec3F{0, 0, 1}})
	{
		EXPECT_EQ(boost::qvm::dot(v, orthogonal(v)), 0);
	}
}
