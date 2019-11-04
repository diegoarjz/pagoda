#include <dynamic_value/dynamic_class.h>
#include <dynamic_value/dynamic_instance.h>
#include <dynamic_value/icallable_body.h>
#include <dynamic_value/null_object_value.h>
#include <dynamic_value/type_info.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace selector;

class TestCallableBody : public ICallableBody
{
public:
	TestCallableBody() : m_called(false) {}
	virtual ~TestCallableBody() {}

	void Call(const std::vector<DynamicValueBasePtr>& args) override { m_called = true; }
	void SetClosure(const std::shared_ptr<DynamicValueTable>& closure) override { m_closure = closure; }
	const std::shared_ptr<DynamicValueTable>& GetClosure() const override { return m_closure; }

	bool m_called;
	std::shared_ptr<DynamicValueTable> m_closure;
};

class DynamicClassTest : public ::testing::Test
{
public:
	void SetUp() override
	{
		m_constructorCallable = std::make_shared<TestCallableBody>();
		m_dynamicClass = std::make_shared<DynamicClass>("TestClass");
		m_dynamicClass->SetConstructorBody(m_constructorCallable);
	}

	void TearDown() override {}

	std::shared_ptr<TestCallableBody> m_constructorCallable;
	DynamicClassPtr m_dynamicClass;
};

TEST_F(DynamicClassTest, when_calling_a_dynamic_class_should_construct_an_instance)
{
	DynamicValueBasePtr instance = m_dynamicClass->Call({});

	ASSERT_NE(instance, nullptr);
	ASSERT_TRUE(m_constructorCallable->m_called);
	ASSERT_NE(m_constructorCallable->m_closure, nullptr);
	DynamicValueBasePtr closureThis = m_constructorCallable->m_closure->Get("this");

	EXPECT_EQ(closureThis, instance);
}

TEST_F(DynamicClassTest, when_creating_an_instance_from_a_class_its_type_info_should_have_the_class_name)
{
	DynamicValueBasePtr instance = m_dynamicClass->Call({});
	auto typeInfo = instance->GetTypeInfo();
	EXPECT_EQ(typeInfo->GetTypeName(), "TestClass");
}

