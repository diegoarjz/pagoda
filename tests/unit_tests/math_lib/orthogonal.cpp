#include <pagoda/math/vec_base.h>

#include <pagoda/math/orthogonal.h>

#include <boost/qvm/vec_operations.hpp>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::math;

TEST(Orthogonal, when_calculating_the_orthogonal_vector_of_a_2d_vector_should_have_zero_as_dot_product)
{
	boost::qvm::vec<float, 2> v{1, 2};
	ASSERT_EQ(boost::qvm::dot(v, orthogonal(v)), 0);
}

TEST(Orthogonal, when_calculating_the_orthogonal_vector_of_a_3d_vector_should_have_zero_as_dot_product)
{
	boost::qvm::vec<float, 3> v{1, 2, 3};
	ASSERT_EQ(boost::qvm::dot(v, orthogonal(v)), 0);
}

TEST(Orthogonal, when_calculating_the_orthogonal_of_a_vector_parallel_with_an_axis_should_have_zero_as_dot_product)
{
	for (auto v : {boost::qvm::vec<float, 3>{1, 0, 0}, boost::qvm::vec<float, 3>{0, 1, 0}, boost::qvm::vec<float, 3>{0, 0, 1}})
	{
		EXPECT_EQ(boost::qvm::dot(v, orthogonal(v)), 0);
	}
}
