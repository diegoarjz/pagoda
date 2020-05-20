#include <pagoda/dynamic/binding/cast_to.h>
#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/dynamic_value_table.h>
#include <pagoda/dynamic/value_not_found.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;
using namespace pagoda::dynamic;

class DynamicValueTableTest : public ::testing::Test
{
public:
	void SetUp() override
	{
		m_table = std::make_shared<DynamicValueTable>("table");
		m_parentTable = std::make_shared<DynamicValueTable>("parent");
		m_bool = std::make_shared<Boolean>(true);
	}

	void TearDown() override {}

	std::shared_ptr<DynamicValueTable> m_table;
	std::shared_ptr<DynamicValueTable> m_parentTable;
	BooleanPtr m_bool;
};

TEST_F(DynamicValueTableTest, when_declaring_a_value_should_be_able_to_retrieve_it)
{
	m_table->Declare("b", m_bool);
	auto b = std::dynamic_pointer_cast<Boolean>(m_table->Get("b"));
	ASSERT_NE(b, nullptr);
	ASSERT_TRUE(static_cast<bool>(*b));
}

TEST_F(DynamicValueTableTest, when_assigning_a_value_should_update_its_value)
{
	m_table->Declare("b", m_bool);
	m_table->Assign("b", std::make_shared<Boolean>(false));
	auto b = std::dynamic_pointer_cast<Boolean>(m_table->Get("b"));
	ASSERT_NE(b, nullptr);
	ASSERT_FALSE(static_cast<bool>(*b));
}

TEST_F(DynamicValueTableTest, when_assigning_a_value_that_hasnt_been_declared_should_throw)
{
	try
	{
		m_table->Assign("b", std::make_shared<Boolean>(false));
	}
	catch (ValueNotFoundException &e)
	{
		return;
	}
	FAIL() << "Should have thrown";
}

TEST_F(DynamicValueTableTest, when_getting_a_value_that_hasnt_been_assigned_should_throw)
{
	try
	{
		auto b = m_table->Get("b");
	}
	catch (ValueNotFoundException &e)
	{
		return;
	}
	FAIL() << "Should have thrown";
}

TEST_F(DynamicValueTableTest, when_getting_a_value_should_search_in_parent_tables)
{
	m_table->SetParent(m_parentTable);
	m_parentTable->Declare("b", m_bool);
	auto b = std::dynamic_pointer_cast<Boolean>(m_table->Get("b"));
	ASSERT_NE(b, nullptr);
	ASSERT_TRUE(static_cast<bool>(*b));
}

