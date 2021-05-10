#include "transformation.h"

#include <pagoda/math/math_utils.h>

#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>

#include <gtest/gtest.h>

using namespace pagoda::scene;
using namespace pagoda::math;

TEST(Transformation, test_construction)
{
	Transformation t1;
	Mat4x4F identity = boost::qvm::identity_mat<float, 4>();
	EXPECT_TRUE(t1.GetTransformationMatrix() == identity);

	// Translate
	Transformation t2{Vec3F{0, 0, 0}, {1, 2, 3}, {1, 1, 1}};
	Mat4x4F mat2 = identity;
	boost::qvm::A<0, 3>(mat2) = 1;
	boost::qvm::A<1, 3>(mat2) = 2;
	boost::qvm::A<2, 3>(mat2) = 3;
	EXPECT_TRUE(t2.GetTransformationMatrix() == mat2);

	// Scale
	Transformation t3{Vec3F{0, 0, 0}, {0, 0, 0}, {1, 2, 3}};
	Mat4x4F mat3 = identity;
	boost::qvm::A<0, 0>(mat3) = 1;
	boost::qvm::A<1, 1>(mat3) = 2;
	boost::qvm::A<2, 2>(mat3) = 3;
	EXPECT_TRUE(t3.GetTransformationMatrix() == mat3);

	// Rotate X
	auto radians = MathUtils<float>::degrees_to_radians(90.0f);
	Transformation t4{Vec3F{radians, 0, 0}, {0, 0, 0}, {1, 1, 1}};
	Mat4x4F mat4 = boost::qvm::rotx_mat<4>(radians);
	EXPECT_TRUE(t4.GetTransformationMatrix() == mat4);

	// Rotate Y
	Transformation t5{Vec3F{0, radians, 0}, {0, 0, 0}, {1, 1, 1}};
	Mat4x4F mat5 = boost::qvm::roty_mat<4>(radians);
	EXPECT_TRUE(t5.GetTransformationMatrix() == mat5);

	// Rotate Z
	Transformation t6{Vec3F{0, 0, radians}, {0, 0, 0}, {1, 1, 1}};
	Mat4x4F mat6 = boost::qvm::rotz_mat<4>(radians);
	EXPECT_TRUE(t6.GetTransformationMatrix() == mat6);
}
