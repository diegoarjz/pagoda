#include "compiler.h"

#include "common/assertions.h"
#include "common/profiler.h"

#include "expression.h"
#include "float_parameter.h"
#include "parameter.h"
#include "parameter_validation.h"

#include <regex>
#include <stack>

#include <iostream>

namespace selector
{
ParameterExecutionNode::ParameterExecutionNode(std::shared_ptr<ParameterBase>& parameter) : m_parameter(parameter) {}

void ParameterExecutionNode::AddNextParameter(std::shared_ptr<ParameterExecutionNode> next_node)
{
	m_nextParameters.push_back(next_node);
}

void ParameterExecutionNode::Evaluate()
{
	if (m_parameter->GetExpression() != nullptr)
	{
		m_parameter->GetExpression()->Evaluate();
	}
}

Compiler::Compiler(std::shared_ptr<Context>& root_context)
    : m_rootContext(root_context), m_parameteResolver(root_context)
{
}

bool Compiler::Compile()
{
	START_PROFILE;

	std::stack<std::shared_ptr<Context>> contexts;
	contexts.push(m_rootContext);
	m_rootNodes.clear();
	m_parameterLookup.clear();

	m_compilationError.offending_parameter = nullptr;
	m_compilationError.reason = CompilationFailReason::CompilationSuccessful;

	while (!contexts.empty())
	{
		auto context = contexts.top();
		contexts.pop();

		if (!CompileContext(context))
		{
			return false;
		}

		for (auto child : context->Children())
		{
			contexts.push(child.second);
		}
	}

	// Execution graph creation stage
	for (auto node : m_parameterLookup)
	{
		auto execution_node = node.second;
		auto parameter = execution_node->m_parameter;
		auto expression = parameter->GetExpression();

		if (expression != nullptr)
		{
			auto variables = expression->GetVariables();

			if (variables.size() == 0)
			{
				m_rootNodes.push_back(execution_node);
				continue;
			}

			for (auto v : variables)
			{
				auto referenced_parameter = v.second->GetReferencedParameter();
				m_parameterLookup[referenced_parameter.get()]->AddNextParameter(execution_node);
			}
		}
		else
		{
			m_rootNodes.push_back(execution_node);
		}
	}

	return true;
}

bool Compiler::CompileContext(std::shared_ptr<Context> context)
{
	START_PROFILE;

	auto parameters = context->GetParameters();

	for (auto par : parameters)
	{
		auto parameter = par.second;
		auto parameter_execution_node = std::make_shared<ParameterExecutionNode>(parameter);
		m_parameterLookup.emplace(parameter.get(), parameter_execution_node);

		if (parameter->GetExpression() == nullptr)
		{
			continue;
		}

		if (!CompileExpression(context, parameter))
		{
			return false;
		}
	}

	return true;
}

bool Compiler::CompileExpression(std::shared_ptr<Context> context, std::shared_ptr<ParameterBase> parameter)
{
	START_PROFILE;

	auto expression = parameter->GetExpression();

	auto expression_string = expression->GetExpression();
	std::sregex_token_iterator begin(expression_string.begin(), expression_string.end(),
	                                 ParameterValidation::variable_regex, 1);
	std::sregex_token_iterator end;

	typename Expression::VariableContainer_t variables;
	uint32_t next_variable_id = 0;

	for (auto variables_iterator = begin; variables_iterator != end; ++variables_iterator)
	{
		auto original_variable = variables_iterator->str();

		std::shared_ptr<ExpressionVariable> variable;
		auto iter = variables.find(original_variable);
		if (iter == std::end(variables))
		{
			variable =
			    std::make_shared<ExpressionVariable>("var" + std::to_string(next_variable_id++), original_variable);
			variables.emplace(original_variable, variable);
		}
	}

	auto compiled_expression = expression_string;
	for (auto variable : variables)
	{
		auto original_variable = variable.second->GetOriginalVariable();
		std::regex current_variable_regex("\\$\\{" + original_variable + "\\}");
		compiled_expression =
		    std::regex_replace(compiled_expression, current_variable_regex, variable.second->GetVariableName());

		auto referenced_parameter = m_parameteResolver.Resolve(original_variable, context);

		if (referenced_parameter == nullptr)
		{
			m_compilationError.reason = CompilationFailReason::UnableToResolveParameter;
			m_compilationError.offending_parameter = parameter;
			return false;
		}

		variable.second->SetReferencedParameter(referenced_parameter);
	}
	expression->SetVariables(variables);

	bool compiled;
	switch (parameter->Type())
	{
		case ParameterTypes::Float:
			compiled = CompileFloatExpression(expression, parameter, compiled_expression);
			break;
		default:
			DBG_ASSERT_MSG(false, "Unknown Parameter Type");
			return false;
	}

	return compiled;
}

class FloatExpressionBackend : public Expression::ExpressionBackend
{
public:
	FloatExpressionBackend()
	{ /*expression.register_symbol_table(symbol_table);*/
	}

	void Evaluate() final
	{
		START_PROFILE;

		// expression.value();
	}

	/*
	exprtk::symbol_table<float> symbol_table;
	exprtk::expression<float> expression;
	exprtk::parser<float> parser;
	*/
};  // class FloatExpressionBackend

bool Compiler::CompileFloatExpression(std::shared_ptr<Expression> expression, std::shared_ptr<ParameterBase> parameter,
                                      const std::string& pre_processed_expression)
{
	START_PROFILE;

	auto compiled_expression = "float_out := " + pre_processed_expression;
	expression->SetCompiledExpression(compiled_expression);

	auto backend = std::make_shared<FloatExpressionBackend>();
	expression->SetExpressionBackend(backend);

	// Link output result
	// backend->symbol_table.add_variable("float_out", parameter->Get<FloatParameter>());

	// Link referenced parameters
	for (auto var : expression->GetVariables())
	{
		auto variable = var.second;
		auto referenced_parameter = variable->GetReferencedParameter();

		if (referenced_parameter->Type() != parameter->Type())
		{
			return false;
		}

		// backend->symbol_table.add_variable(variable->GetVariableName(), referenced_parameter->Get<FloatParameter>());
	}

	// Compile the final expression
	// bool compiled = backend->parser.compile(compiled_expression, backend->expression);
	bool compiled = false;
	if (!compiled)
	{
		m_compilationError.reason = CompilationFailReason::InvalidSyntax;
		m_compilationError.offending_parameter = parameter;
		return false;
	}

	return true;
}

}  // namespace selector
