#include <geometry_core/indexed_container.h>
#include <math_lib/vec_base.h>

#include <list>

#include <gtest/gtest.h>

using namespace selector;

template<class T>
class IndexedContainerTest : public ::testing::Test
{
};

typedef ::testing::Types<IndexedContainer<uint32_t, Vec3F>, AssociativeIndexedContainer<uint32_t, Vec3F>>
    IndexedContainerTypes;

TYPED_TEST_SUITE(IndexedContainerTest, IndexedContainerTypes);

TYPED_TEST(IndexedContainerTest, when_creating_should_return_incremental_indices)
{
	TypeParam c;

	EXPECT_EQ(c.Create(0, 0, 0), 0);
	EXPECT_EQ(c.Create(0, 0, 0), 1);
	EXPECT_EQ(c.Create(0, 0, 0), 2);
}

TYPED_TEST(IndexedContainerTest, when_getting_based_on_index_should_return_correct_element)
{
	TypeParam c;

	uint32_t index = c.Create(1, 2, 3);
	uint32_t index2 = c.Create(3, 2, 1);

	EXPECT_EQ(c.Get(index), Vec3F(1, 2, 3));
	EXPECT_EQ(c.Get(index2), Vec3F(3, 2, 1));
}

TYPED_TEST(IndexedContainerTest, when_getting_based_on_index_should_return_by_value)
{
	TypeParam c;

	uint32_t index = c.Create(1, 2, 3);

	Vec3F &value = c.Get(index);
	EXPECT_EQ(value, Vec3F(1, 2, 3));
}

TYPED_TEST(IndexedContainerTest, when_changing_value_of_element_retrieved_by_index_should_updated_value_in_container)
{
	TypeParam c;

	uint32_t index = c.Create(1, 2, 3);
	c.Get(index)[0] = 4;
	EXPECT_EQ(c.Get(index), Vec3F(4, 2, 3));
}

TEST(IndexedContainerIteratorTest, when_iterating_should_visit_all_values)
{
	IndexedContainer<uint32_t, uint32_t> c;

	std::list<uint32_t> values{0, 1, 2, 3};

	for (auto v : values)
	{
		c.Create(v);
	}

	for (auto &v : c)
	{
		ASSERT_NE(std::find(values.begin(), values.end(), v), std::end(values));
		values.remove(v);
	}

	ASSERT_EQ(values.size(), 0);
}

TEST(IndexedContainerIteratorTest, when_iterating_indices_should_visit_all_indices)
{
	IndexedContainer<uint32_t, uint32_t> c;

	std::list<uint32_t> values{0, 1, 2, 3};

	for (auto v : values)
	{
		c.Create(v);
	}

	size_t i = 0;
	for (auto iter = c.index_begin(); iter != c.index_end(); ++iter)
	{
		EXPECT_EQ(*iter, i++);
	}
	EXPECT_EQ(i, 4);
}

TEST(AssociativeIndexedContainersTest, when_iterating_should_visit_all_values)
{
	AssociativeIndexedContainer<uint32_t, uint32_t> c;

	std::list<uint32_t> values{0, 1, 2, 3};

	for (auto v : values)
	{
		c.Create(v);
	}

	for (auto &v : c)
	{
		ASSERT_NE(std::find(values.begin(), values.end(), v.second), std::end(values));
		values.remove(v.second);
	}

	ASSERT_EQ(values.size(), 0);
}
