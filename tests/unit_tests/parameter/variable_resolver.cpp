#include <parameter/parameter.h>
#include <parameter/variable.h>
#include <parameter/variable_resolver.h>

#include <gtest/gtest.h>

using namespace selector;

class VariableResolverTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_rootContext = std::make_shared<Context>("root");
		m_rootContext->SetParameter("par1", std::make_shared<FloatValue>(123.0f));

		auto context = std::make_shared<Context>("context1");
		context->SetParameter("par2", std::make_shared<FloatValue>(321.0f));

		m_rootContext->SetParameter("c1", context);
	}

	void TearDown() {}

	std::shared_ptr<Context> m_rootContext;
};

TEST_F(VariableResolverTest, when_resolving_a_simple_variable_should_get_the_parameter_in_the_given_parameterizable)
{
	VariableResolver r(Variable("par1"));
	auto resolved = r.Resolve(m_rootContext);

	EXPECT_EQ(get_parameter_as<float>(resolved), 123.0f);
}

TEST_F(VariableResolverTest, when_resolving_a_compound_variable_should_go_through_the_parameterizable_hierarchy)
{
	VariableResolver r(Variable("c1.par2"));
	auto resolved = r.Resolve(m_rootContext);
	EXPECT_EQ(get_parameter_as<float>(resolved), 321.0f);
}

TEST_F(VariableResolverTest, when_unable_to_resolve_a_variable_should_throw)
{
	VariableResolver r(Variable("par2"));
	auto resolved = r.Resolve(m_rootContext);
}
