#include <math_lib/vec_base.h>

#include <math_lib/cross_product.h>
#include <math_lib/dot_product.h>
#include <math_lib/length.h>
#include <math_lib/normalize.h>

#include <iterator>

#include <gtest/gtest.h>

using namespace selector;

TEST(VecBase, when_using_default_constructor_should_initizalize_all_elements_to_zero)
{
	VecBase<3, float> zero;

	EXPECT_EQ(zero[0], 0);
	EXPECT_EQ(zero[1], 0);
	EXPECT_EQ(zero[2], 0);
}

TEST(VecBase, when_using_values_constructor_should_initialize_all_elements_accordingly)
{
	VecBase<3, float> v(1, 2, 3);

	EXPECT_EQ(v[0], 1);
	EXPECT_EQ(v[1], 2);
	EXPECT_EQ(v[2], 3);
}

TEST(VecBase, when_using_value_constructor_should_initialize_all_elements_to_value)
{
	VecBase<3, float> v(1.23f);

	EXPECT_EQ(v[0], 1.23f);
	EXPECT_EQ(v[1], 1.23f);
	EXPECT_EQ(v[2], 1.23f);
}

TEST(VecBase, when_using_copy_constructor_should_copy_all_elements)
{
	VecBase<3, float> rhs(1.0f, 2.0f, 4.2f);
	VecBase<3, float> lhs = rhs;

	EXPECT_EQ(lhs[0], 1.0f);
	EXPECT_EQ(lhs[1], 2.0f);
	EXPECT_EQ(lhs[2], 4.2f);
}

TEST(VecBase, when_assigning_should_update_all_values)
{
	VecBase<3, float> lhs(0, 0, 0);
	VecBase<3, float> rhs(1, 2, 3);

	lhs = rhs;

	EXPECT_EQ(lhs[0], 1);
	EXPECT_EQ(lhs[1], 2);
	EXPECT_EQ(lhs[2], 3);
}

TEST(VecBase, when_dereferencing_should_return_an_array)
{
	VecBase<3, float> vec(2, 3, 1);
	float *elementPtr = static_cast<float *>(vec);

	float expected[] = {2, 3, 1};

	for (auto j = 0u; j < 3; ++j)
	{
		EXPECT_EQ(elementPtr[j], expected[j]);
	}
}

TEST(VecBase, when_adding_two_vectors_should_add_each_component)
{
	VecBase<3, float> lhs(4, 4, 4);
	VecBase<3, float> rhs(1, 2, 3);
	auto result = lhs + rhs;
	VecBase<3, float> expected(5, 6, 7);

	EXPECT_EQ(result, expected);
}

TEST(VecBase, when_subtracting_two_vectors_should_subtract_each_component)
{
	VecBase<3, float> lhs(4, 4, 4);
	VecBase<3, float> rhs(1, 2, 3);
	auto result = lhs - rhs;
	VecBase<3, float> expected(3, 2, 1);

	EXPECT_EQ(result, expected);
}

TEST(VecBase, when_multiplying_by_a_scalar_should_multiply_all_elements_by_the_scalar)
{
	VecBase<3, float> vec(1, 2, 3);
	VecBase<3, float> expected(2, 4, 6);

	EXPECT_EQ((2 * vec), expected);
	EXPECT_EQ((vec * 2), expected);
}

TEST(VecBase, when_dividing_by_a_scalar_should_divide_all_elements_by_the_scalar)
{
	VecBase<3, float> vec(2, 4, 6);
	VecBase<3, float> expected(1, 2, 3);

	EXPECT_EQ((vec / 2), expected);
}

TEST(VecBase, when_dividing_by_a_scalar_on_the_left_should_divide_the_scalar_by_each_element)
{
	VecBase<3, float> vec(2, 4, 6);
	VecBase<3, float> expected(1, 2.0f / 4.0f, 2.0f / 6.0f);
	EXPECT_EQ((2 / vec), expected);
}

TEST(VecOperations, when_using_squared_length_on_a_vector_should_return_the_squared_length_of_the_vector)
{
	VecBase<3, float> vec(1, 1, 1);

	EXPECT_EQ(squared_length(vec), 3);
}

TEST(VecOperations, when_using_length_on_a_vector_should_return_the_lenght_of_the_vector)
{
	VecBase<3, float> vec(1, 1, 1);
	EXPECT_EQ(length(vec), std::sqrt(3.0f));
}

TEST(VecOperations, when_using_normalized_on_a_vector_should_return_the_normalized_vector)
{
	VecBase<3, float> vec(1, 1, 1);
	VecBase<3, float> expected(1 / std::sqrt(3.0f));

	EXPECT_EQ(normalized(vec), expected);
	EXPECT_EQ(vec[0], 1);
	EXPECT_EQ(vec[1], 1);
	EXPECT_EQ(vec[2], 1);

	normalize(vec);
	EXPECT_EQ(vec, expected);
	EXPECT_GE(length(vec), 0.99999);
	EXPECT_LE(length(vec), 1.00001);
}

TEST(VecOperations, when_using_cross_product_should_return_the_cross_product_of_the_two_vectors)
{
	VecBase<3, float> X(1, 0, 0);
	VecBase<3, float> Y(0, 1, 0);
	VecBase<3, float> Z(0, 0, 1);

	EXPECT_EQ(cross_product(X, Y), Z);
	EXPECT_EQ(cross_product(Y, Z), X);
	EXPECT_EQ(cross_product(Z, X), Y);
}

TEST(VecOperations, when_using_dot_product_should_return_the_dot_product_of_the_two_vectors)
{
	VecBase<3, float> v1(1, 2, 3);
	VecBase<3, float> v2(4, 5, 6);
	EXPECT_EQ(dot_product(v1, v2), 32);
}

TEST(VecOperations, when_calculating_the_dot_product_of_orthogonal_vectors_should_return_0)
{
	VecBase<3, float> v1(1, 0, 0);
	VecBase<3, float> v2(0, 2, 0);
	EXPECT_EQ(dot_product(v1, v2), 0.0f);
}
