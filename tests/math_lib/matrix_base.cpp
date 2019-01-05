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
