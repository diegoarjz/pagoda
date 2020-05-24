#include <math_lib/matrix_base.h>
#include <math_lib/transpose.h>

#include <gtest/gtest.h>

using namespace pagoda;

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

TEST(MatrixBase, when_constructing_from_a_smaller_matrix_should_copy_with_a_diagonal)
{
	MatrixBase<2, 2, float> m2x2;
	m2x2.SetRow(0, boost::qvm::vec<float, 2>(1, 2));
	m2x2.SetRow(1, boost::qvm::vec<float, 2>(3, 4));
	MatrixBase<3, 3, float> m3x3(m2x2);
	MatrixBase<3, 3, float> expected;
	expected.SetRow(0, boost::qvm::vec<float, 3>(1, 2, 0));
	expected.SetRow(1, boost::qvm::vec<float, 3>(3, 4, 0));
	expected.SetRow(2, boost::qvm::vec<float, 3>(0, 0, 1));
	EXPECT_EQ(m3x3, expected);
}

TEST(MatrixBase, when_constructing_from_a_larger_matrix_should_copy_the_upper_left_block)
{
	MatrixBase<3, 3, float> m3x3;
	m3x3.SetRow(0, boost::qvm::vec<float, 3>(1, 2, 3));
	m3x3.SetRow(1, boost::qvm::vec<float, 3>(4, 5, 6));
	m3x3.SetRow(2, boost::qvm::vec<float, 3>(7, 8, 9));
	MatrixBase<2, 2, float> m2x2(m3x3);
	MatrixBase<2, 2, float> expected;
	expected.SetRow(0, boost::qvm::vec<float, 2>(1, 2));
	expected.SetRow(1, boost::qvm::vec<float, 2>(4, 5));
	EXPECT_EQ(m2x2, expected);
}

TEST(MatrixBase, when_using_setters_should_update_values_accordingly)
{
	MatrixBase<2, 2, float> matrix;

	matrix.Value(0, 0) = 1;
	matrix.Value(1, 0) = 2;
	matrix.Value(0, 1) = 3;
	matrix.Value(1, 1) = 4;

	ASSERT_EQ(matrix.Col(0), boost::qvm::vec<float, 2>(1, 3));
	ASSERT_EQ(matrix.Col(1), boost::qvm::vec<float, 2>(2, 4));
	ASSERT_EQ(matrix.Row(0), boost::qvm::vec<float, 2>(1, 2));
	ASSERT_EQ(matrix.Row(1), boost::qvm::vec<float, 2>(3, 4));
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

	ASSERT_EQ(r.Col(0), boost::qvm::vec<float, 2>(2, 6));
	ASSERT_EQ(r.Col(1), boost::qvm::vec<float, 2>(4, 8));
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

	ASSERT_EQ(r.Col(0), boost::qvm::vec<float, 2>(0, 0));
	ASSERT_EQ(r.Col(1), boost::qvm::vec<float, 2>(0, 0));
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

	ASSERT_EQ(r.Col(0), boost::qvm::vec<float, 2>(8, 20));
	ASSERT_EQ(r.Col(1), boost::qvm::vec<float, 2>(5, 13));
}

TEST(MatrixBase, when_multiplying_matrices_by_scalars_should_produce_correct_values)
{
	MatrixBase<2, 2, float> m1;

	m1.Value(0, 0) = 1;
	m1.Value(1, 0) = 2;
	m1.Value(0, 1) = 3;
	m1.Value(1, 1) = 4;

	auto r = 2.0f * m1 * 3.0f;

	ASSERT_EQ(r.Col(0), boost::qvm::vec<float, 2>(6, 18));
	ASSERT_EQ(r.Col(1), boost::qvm::vec<float, 2>(12, 24));
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

	ASSERT_EQ(r, boost::qvm::vec<float, 2>(4, 6));
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

	ASSERT_EQ(r, boost::qvm::vec<float, 2>(3, 7));
}

TEST(MatrixBase, when_right_multiplying_a_vector_with_a_translation_matrix_should_translate_the_vector)
{
	boost::qvm::mat<float, 4, 4> m = translate_matrix<float>(1, 2, 3);
	boost::qvm::vec<float, 4> p(-5.0, -5.0, -2.5, 1.0);

	auto r = m * p;
	EXPECT_EQ(r, boost::qvm::vec<float, 4>(-4.0f, -3.0f, 0.5f, 1.0f));
}

TEST(MatrixBase, when_transposing_a_matrix_should_swap_rows_with_columns)
{
	boost::qvm::mat<float, 2, 2> m;
	m.SetRow(0, boost::qvm::vec<float, 2>(1, 2));
	m.SetRow(1, boost::qvm::vec<float, 2>(3, 4));
	m = transpose(m);
	boost::qvm::mat<float, 2, 2> expected;
	expected.SetRow(0, boost::qvm::vec<float, 2>(1, 3));
	expected.SetRow(1, boost::qvm::vec<float, 2>(2, 4));
	EXPECT_EQ(m, expected);
}
