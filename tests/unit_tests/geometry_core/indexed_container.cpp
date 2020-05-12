#include <pagoda/geometry_core/indexed_container.h>
#include <pagoda/math_lib/vec_base.h>

#include <iostream>
#include <list>

#include <gtest/gtest.h>

using namespace pagoda;

class Value
{
public:
	static const Value s_invalidValue;

	Value() : m_value(s_invalidValue.m_value) {}
	Value(const std::size_t &v) : m_value(v) {}
	std::size_t m_value;

	bool operator==(const Value &o) const { return m_value == o.m_value; }
	bool operator!=(const Value &o) const { return m_value != o.m_value; }
};
const Value Value::s_invalidValue(std::numeric_limits<std::size_t>::max());

TEST(IndexedContainerTest, when_creating_values_should_return_incremental_indices)
{
	IndexedContainer<std::size_t, Value> c;
	EXPECT_EQ(c.Create(1), 0);
	EXPECT_EQ(c.Create(1), 1);
}

TEST(IndexedContainerTest, when_creating_values_should_store_the_values)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Create(2);
	EXPECT_EQ(c.Get(0).m_value, 1);
	EXPECT_EQ(c.Get(1).m_value, 2);
}

TEST(IndexedContainerTest, when_creating_and_getting_should_return_the_index_and_value)
{
	IndexedContainer<std::size_t, Value> c;
	auto e1 = c.CreateAndGet(1);
	EXPECT_EQ(e1.m_index, 0);
	EXPECT_EQ(e1.m_value.m_value, 1);

	auto e2 = c.CreateAndGet(2);
	EXPECT_EQ(e2.m_index, 1);
	EXPECT_EQ(e2.m_value.m_value, 2);
}

TEST(IndexedContainerTest, when_checking_if_has_index_should_check_for_existence)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Create(2);

	EXPECT_TRUE(c.HasIndex(0));
	EXPECT_TRUE(c.HasIndex(1));
	EXPECT_FALSE(c.HasIndex(2));
}

TEST(IndexedContainerTest, when_accessing_a_deleted_index_should_throw)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Delete(0);
	try
	{
		c.Get(0);
	}
	catch (IndexedDeletedException<std::size_t> &e)
	{
		return;
	}
	FAIL() << "Should have thrown";
}

TEST(IndexedContainerTest, when_checking_if_has_index_of_a_deleted_index_should_return_false)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Create(2);
	c.Delete(0);
	EXPECT_FALSE(c.HasIndex(0));
	EXPECT_TRUE(c.HasIndex(1));
}

TEST(IndexedContainerTest, when_getting_or_creating_should_create_if_doesnt_exist)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	auto value = c.GetOrCreate(2, 123);
	EXPECT_EQ(value.m_value, 123);
}

TEST(IndexedContainerTest, when_creating_a_new_element_after_getting_or_creating_should_use_the_next_available_index)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.GetOrCreate(2, 123);
	auto index = c.Create(321);
	auto value = c.Get(index);
	EXPECT_EQ(index, 1);
	EXPECT_EQ(value, 321);
}

TEST(IndexedContainerTest, when_getting_the_count_of_elements_should_not_take_into_account_the_deleted_indices)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Create(2);
	c.Create(3);
	c.Delete(0);

	EXPECT_EQ(c.Count(), 2);
}

TEST(IndexedContainerTest, when_iterating_should_iterate_indices_in_order)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Create(2);
	c.Create(3);

	auto i = 0u;
	for (const auto &el : c)
	{
		EXPECT_EQ(el.m_index, i);
		EXPECT_EQ(el.m_value.m_value, i + 1);
		++i;
	}
}

TEST(IndexedContainerTest, when_iterating_should_skip_the_deleted_indices)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Create(2);
	c.Create(3);
	c.Delete(0);

	auto i = 1u;  // index 0 has been deleted
	for (const auto el : c)
	{
		EXPECT_EQ(el.m_index, i);
		EXPECT_EQ(el.m_value.m_value, i + 1);
		++i;
	}
}

TEST(IndexedContainerTest, when_iterating_should_skip_the_invalid_indices)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Create(2);

	auto i = 0u;
	for (const auto el : c)
	{
		EXPECT_EQ(el.m_index, i);
		EXPECT_EQ(el.m_value.m_value, i + 1);
		++i;
	}
}

TEST(IndexedContainerTest, when_getting_an_element_should_be_able_to_change_the_value)
{
	IndexedContainer<std::size_t, Value> c;
	auto index = c.Create(1);
	auto &value = c.Get(index);
	value.m_value = 123;
	EXPECT_EQ(c.Get(0).m_value, 123);
}

TEST(IndexedContainerTest, when_changing_values_through_an_iterator_should_change_the_stored_value)
{
	IndexedContainer<std::size_t, Value> c;
	c.Create(1);
	c.Create(2);

	for (const auto &el : c)
	{
		el.m_value = 0;
	}

	EXPECT_EQ(c.Get(0).m_value, 0);
	EXPECT_EQ(c.Get(1).m_value, 0);
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
