#include <math_lib/orthogonal.cpp>

#include <gtest/gtest.h>

TEST(Orthogonal, when_calculating_the_orthogonal_vector_of_a_2d_vector_should_have_zero_as_dot_product)
{
    Vec2F v(1,2);
    ASSERT_EQ(dot_product(v, orthogonal(v)), 0);
}

TEST(Orthogonal, when_calculating_the_orthogonal_vector_of_a_3d_vector_should_have_zero_as_dot_product)
{
    Vec3F v(1,2,3);
    ASSERT_EQ(dot_product(v, orthogonal(v)), 0);
}