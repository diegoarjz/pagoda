#include <parameter/compiler.h>
#include <parameter/context.h>
#include <parameter/expression.h>
#include <parameter/float_parameter.h>
#include <parameter/parameter.h>
#include <parameter/parameter_identifier.h>

#include <gtest/gtest.h>
#include <queue>

using namespace selector;

class CompilerTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		single_parameter_context = std::make_shared<Context>("context");
		par1 = single_parameter_context->CreateParameter<FloatParameter>("par", 5.0f);

		two_parameters_context = std::make_shared<Context>("child");
		par2 = two_parameters_context->CreateParameter<FloatParameter>("par2", 0.0f);
		par3 = two_parameters_context->CreateParameter<FloatParameter>("par3", 3.0f);

		two_parameters_context->SetParent(single_parameter_context);
	}

	void TearDown() {}

	void Execute(const std::list<std::shared_ptr<ParameterExecutionNode>> &nodes)
	{
		std::queue<std::shared_ptr<ParameterExecutionNode>> exec_queue;

		for (auto n : nodes)
		{
			exec_queue.push(n);
		}

		while (!exec_queue.empty())
		{
			auto front = exec_queue.front();
			exec_queue.pop();

			front->Evaluate();

			for (auto next_node : front->GetDependantNodes())
			{
				exec_queue.push(next_node.lock());
			}
		}
	}

	std::shared_ptr<Context> single_parameter_context;
	std::shared_ptr<ParameterBase> par1;

	std::shared_ptr<Context> two_parameters_context;
	std::shared_ptr<ParameterBase> par2;
	std::shared_ptr<ParameterBase> par3;
};

TEST_F(CompilerTest, test_single_parameter_no_references)
{
	auto expression = std::make_shared<Expression>("2 + 5");
	par1->SetExpression(expression);

	Compiler compiler(single_parameter_context);
	EXPECT_TRUE(compiler.Compile());
	Execute(compiler.GetRootNodes());

	EXPECT_EQ(par1->Get<FloatParameter>(), 7);
	EXPECT_EQ(compiler.GetErrorInfo().reason, Compiler::CompilationFailReason::CompilationSuccessful);
}

TEST_F(CompilerTest, test_two_parameters_reference)
{
	auto expression = std::make_shared<Expression>("${par3} + ${par3}");
	par2->SetExpression(expression);

	Compiler compiler(two_parameters_context);
	EXPECT_TRUE(compiler.Compile());
	Execute(compiler.GetRootNodes());

	EXPECT_EQ(par2->Get<FloatParameter>(), 6);
	EXPECT_EQ(compiler.GetErrorInfo().reason, Compiler::CompilationFailReason::CompilationSuccessful);
}

TEST_F(CompilerTest, test_parameter_references_other_contexts)
{
	auto expression = std::make_shared<Expression>("${../par} * ${par3}");
	par2->SetExpression(expression);

	Compiler compiler(single_parameter_context);
	EXPECT_TRUE(compiler.Compile());
	Execute(compiler.GetRootNodes());

	EXPECT_EQ(par2->Get<FloatParameter>(), 15);
	EXPECT_EQ(compiler.GetErrorInfo().reason, Compiler::CompilationFailReason::CompilationSuccessful);
}

TEST_F(CompilerTest, test_parameter_reference_and_constants)
{
	auto expression = std::make_shared<Expression>("10*${par3}");
	par2->SetExpression(expression);

	Compiler compiler(two_parameters_context);
	EXPECT_TRUE(compiler.Compile());
	Execute(compiler.GetRootNodes());

	EXPECT_EQ(par2->Get<FloatParameter>(), 30);
	EXPECT_EQ(compiler.GetErrorInfo().reason, Compiler::CompilationFailReason::CompilationSuccessful);
}

TEST_F(CompilerTest, test_unable_to_compile_cant_resolve_parameter)
{
	auto expression = std::make_shared<Expression>("${par50}");
	par1->SetExpression(expression);

	Compiler compiler(single_parameter_context);
	EXPECT_FALSE(compiler.Compile());
	EXPECT_EQ(compiler.GetErrorInfo().reason, Compiler::CompilationFailReason::UnableToResolveParameter);
	EXPECT_EQ(compiler.GetErrorInfo().offending_parameter, par1);
}

TEST_F(CompilerTest, test_unable_to_compile_invalid_syntax)
{
	auto expression = std::make_shared<Expression>("1*");
	par1->SetExpression(expression);

	Compiler compiler(single_parameter_context);
	EXPECT_FALSE(compiler.Compile());
	EXPECT_EQ(compiler.GetErrorInfo().reason, Compiler::CompilationFailReason::InvalidSyntax);
	EXPECT_EQ(compiler.GetErrorInfo().offending_parameter, par1);
}
