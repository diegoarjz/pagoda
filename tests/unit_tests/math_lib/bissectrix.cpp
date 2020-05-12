#include <pagoda/math/bissectrix.h>
#include <pagoda/math/vec_base.h>

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::math;

TEST(Bissectrix, when_finding_the_bissectrix_between_two_vectors_should_return_a_vector)
{
	boost::qvm::vec<float, 3> v1{1, 1, 0};
	boost::qvm::vec<float, 3> v2{-1, 1, 0};

	auto r = bissectrix(v1, v2);
	EXPECT_TRUE(r == (Vec3F{0, 1, 0}));
}

TEST(Bissectrix, when_finding_the_bissectrix_between_two_colinear_vectors_should_return_an_orthogonal_vector)
{
	boost::qvm::vec<float, 3> v1{1, 0, 0};
	boost::qvm::vec<float, 3> v2{-1, 0, 0};

	auto r = bissectrix(v1, v2);

	EXPECT_EQ(boost::qvm::dot(r, v1), 0.0f);
}
