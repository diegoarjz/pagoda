#include <math_lib/matrix_base.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(MatrixBase, when_constructing_with_default_constructor_should_set_all_elements_to_zero)
{
	MatrixBase<3, 3, float> zero;

	for (auto r = 0; r < 3; ++r)
	{
		for (auto c = 0; c < 3; ++c)
		{
			ASSERT_EQ(zero.Value(c, r), 0);
		}
	}
}

TEST(MatrixBase, when_constructing_with_diagonal_constructor_should_set_diagonal_to_value)
{
	MatrixBase<3, 3, float> diag(1);

	for (auto r = 0; r < 3; ++r)
	{
		for (auto c = 0; c < 3; ++c)
		{
			if (c == r)
			{
				ASSERT_EQ(diag.Value(c, r), 1);
			}
			else
			{
				ASSERT_EQ(diag.Value(c, r), 0);
			}
		}
	}
}

TEST(MatrixBase, when_using_setters_should_update_values_accordingly)
{
	MatrixBase<2, 2, float> matrix;

	matrix.Value(0, 0) = 1;
	matrix.Value(1, 0) = 2;
	matrix.Value(0, 1) = 3;
	matrix.Value(1, 1) = 4;

	ASSERT_EQ(matrix.Col(0), Vec2F(1, 3));
	ASSERT_EQ(matrix.Col(1), Vec2F(2, 4));
	ASSERT_EQ(matrix.Row(0), Vec2F(1, 2));
	ASSERT_EQ(matrix.Row(1), Vec2F(3, 4));
}

TEST(MatrixBase, when_adding_two_matrices_should_produce_correct_values)
{
	MatrixBase<2, 2, float> m1;
	MatrixBase<2, 2, float> m2;

	m1.Value(0, 0) = 1;
	m1.Value(1, 0) = 2;
	m1.Value(0, 1) = 3;
	m1.Value(1, 1) = 4;

	m2.Value(0, 0) = 1;
	m2.Value(1, 0) = 2;
	m2.Value(0, 1) = 3;
	m2.Value(1, 1) = 4;

	auto r = m1 + m2;

	ASSERT_EQ(r.Col(0), Vec2F(2, 6));
	ASSERT_EQ(r.Col(1), Vec2F(4, 8));
}

TEST(MatrixBase, when_subtracting_two_matrices_should_produce_correct_values)
{
	MatrixBase<2, 2, float> m1;
	MatrixBase<2, 2, float> m2;

	m1.Value(0, 0) = 1;
	m1.Value(1, 0) = 2;
	m1.Value(0, 1) = 3;
	m1.Value(1, 1) = 4;

	m2.Value(0, 0) = 1;
	m2.Value(1, 0) = 2;
	m2.Value(0, 1) = 3;
	m2.Value(1, 1) = 4;

	auto r = m1 - m2;

	ASSERT_EQ(r.Col(0), Vec2F(0, 0));
	ASSERT_EQ(r.Col(1), Vec2F(0, 0));
}

TEST(MatrixBase, when_multiplying_to_matrices_should_produce_correct_values)
{
	MatrixBase<2, 2, float> m1;
	MatrixBase<2, 2, float> m2;

	m1.Value(0, 0) = 1;
	m1.Value(1, 0) = 2;
	m1.Value(0, 1) = 3;
	m1.Value(1, 1) = 4;

	m2.Value(0, 0) = 4;
	m2.Value(1, 0) = 3;
	m2.Value(0, 1) = 2;
	m2.Value(1, 1) = 1;

	auto r = m1 * m2;

	ASSERT_EQ(r.Col(0), Vec2F(8, 20));
	ASSERT_EQ(r.Col(1), Vec2F(5, 13));
}

TEST(MatrixBase, when_multiplying_matrices_by_scalars_should_produce_correct_values)
{
	MatrixBase<2, 2, float> m1;

	m1.Value(0, 0) = 1;
	m1.Value(1, 0) = 2;
	m1.Value(0, 1) = 3;
	m1.Value(1, 1) = 4;

	auto r = 2.0f * m1 * 3.0f;

	ASSERT_EQ(r.Col(0), Vec2F(6, 18));
	ASSERT_EQ(r.Col(1), Vec2F(12, 24));
}

TEST(MatrixBase, when_left_multiplying_a_vector_with_a_matrix_should_produce_correct_values)
{
	MatrixBase<2, 2, float> m1;
	VecBase<2, float> v(1, 1);

	m1.Value(0, 0) = 1;
	m1.Value(1, 0) = 2;
	m1.Value(0, 1) = 3;
	m1.Value(1, 1) = 4;

	VecBase<2, float> r = v * m1;

	ASSERT_EQ(r, Vec2F(4, 6));
}

TEST(MatrixBase, when_right_multiplying_a_vector_with_a_matrix_should_produce_correct_values)
{
	MatrixBase<2, 2, float> m1;
	VecBase<2, float> v(1, 1);

	m1.Value(0, 0) = 1;
	m1.Value(1, 0) = 2;
	m1.Value(0, 1) = 3;
	m1.Value(1, 1) = 4;

	VecBase<2, float> r = m1 * v;

	ASSERT_EQ(r, Vec2F(3, 7));
}
