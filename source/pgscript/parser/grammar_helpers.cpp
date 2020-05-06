#include "grammar_helpers.h"

#include "common/exception.h"

#include <memory>

namespace pagoda
{
ast::IdentifierPtr make_identifier(const std::vector<char> &identifier, const std::vector<char> &identifier2)
{
    std::vector<char> concatIdentifiers = identifier;
    concatIdentifiers.reserve(identifier.size() + identifier2.size());
    for (auto c : identifier2)
    {
        concatIdentifiers.push_back(c);
    }
	return std::make_shared<ast::Identifier>(concatIdentifiers);
}

ast::FloatPtr make_float(const float &f) { return std::make_shared<ast::Float>(f); }

ast::IntegerPtr make_int(const int &i) { return std::make_shared<ast::Integer>(i); }

ast::StringPtr make_string(const std::vector<char> &s)
{
	return std::make_shared<ast::String>(std::string(s.begin(), s.end()));
}

ast::BooleanPtr make_true() { return std::make_shared<ast::Boolean>(true); }

ast::BooleanPtr make_false() { return std::make_shared<ast::Boolean>(false); }

ast::Nullptr make_null() { return std::make_shared<ast::Null>(); }

ast::UnaryPtr make_unary_op(char op, const ast::ExpressionPtr &rhs)
{
	switch (op)
	{
		case '!':
			return std::make_shared<ast::Unary>(ast::Unary::types::Neg, rhs);
		case '-':
			return std::make_shared<ast::Unary>(ast::Unary::types::Min, rhs);
	}
	throw Exception("Invalid operator");
}

ast::ArithmeticOpPtr make_arithmetic_op(char op, const ast::ExpressionPtr &lhs, const ast::ExpressionPtr &rhs)
{
	switch (op)
	{
		case '+':
			return std::make_shared<ast::ArithmeticOp>(ast::ArithmeticOp::types::Add, lhs, rhs);
		case '-':
			return std::make_shared<ast::ArithmeticOp>(ast::ArithmeticOp::types::Sub, lhs, rhs);
		case '*':
			return std::make_shared<ast::ArithmeticOp>(ast::ArithmeticOp::types::Mul, lhs, rhs);
		case '/':
			return std::make_shared<ast::ArithmeticOp>(ast::ArithmeticOp::types::Div, lhs, rhs);
	}
	throw Exception("Invalid operator");
}

ast::ComparisonOpPtr make_comparison_op(const std::string &op, const ast::ExpressionPtr &lhs,
                                        const ast::ExpressionPtr &rhs)
{
	if (op == "==")
	{
		return std::make_shared<ast::ComparisonOp>(ast::ComparisonOp::types::Eq, lhs, rhs);
	}
	if (op == "!=")
	{
		return std::make_shared<ast::ComparisonOp>(ast::ComparisonOp::types::Ne, lhs, rhs);
	}
	if (op == ">")
	{
		return std::make_shared<ast::ComparisonOp>(ast::ComparisonOp::types::Gt, lhs, rhs);
	}
	if (op == ">=")
	{
		return std::make_shared<ast::ComparisonOp>(ast::ComparisonOp::types::Gte, lhs, rhs);
	}
	if (op == "<")
	{
		return std::make_shared<ast::ComparisonOp>(ast::ComparisonOp::types::Lt, lhs, rhs);
	}
	if (op == "<=")
	{
		return std::make_shared<ast::ComparisonOp>(ast::ComparisonOp::types::Lte, lhs, rhs);
	}
	throw Exception("Invalid operator");
}

ast::LogicOpPtr make_logic_op(const std::string &op, const ast::ExpressionPtr &lhs, const ast::ExpressionPtr &rhs)
{
	if (op == "and")
	{
		return std::make_shared<ast::LogicOp>(ast::LogicOp::types::And, lhs, rhs);
	}
	if (op == "or")
	{
		return std::make_shared<ast::LogicOp>(ast::LogicOp::types::Or, lhs, rhs);
	}
	throw Exception("Invalid operator");
}

ast::ExpressionPtr make_expression(const ast::ExpressionPtr &expression) { return expression; }

ast::ExpressionStatementPtr make_expression_statment(const ast::ExpressionPtr &expression)
{
	return std::make_shared<ast::ExpressionStatement>(expression);
}

ast::AssignmentPtr make_assignment(const ast::IdentifierPtr &identifier, const ast::ExpressionPtr &rhs)
{
	return std::make_shared<ast::Assignment>(identifier, rhs);
}

ast::LoopPtr make_while_loop(const ast::ExpressionPtr &condition, const ast::StatementPtr body)
{
	return std::make_shared<ast::Loop>(condition, body);
}

ast::IfStatementPtr make_if_statement(ast::ExpressionPtr &condition, ast::StatementPtr &trueStatement)
{
	return std::make_shared<ast::IfStatement>(condition, trueStatement, nullptr);
}

ast::IfStatementPtr &add_else_statement(ast::IfStatementPtr &ifStatement, ast::StatementPtr &elseStatement)
{
	ifStatement->SetFalseStatement(elseStatement);
	return ifStatement;
}

ast::StatementBlockPtr make_statement_block() { return std::make_shared<ast::StatementBlock>(); }

void add_statement(ast::StatementBlockPtr &block, ast::StatementPtr &statement) { block->AddStatement(statement); }

ast::VarDeclPtr make_var_decl(ast::IdentifierPtr &identifier, boost::optional<ast::ExpressionPtr> &initializer)
{
	if (initializer)
	{
		return std::make_shared<ast::VarDecl>(identifier, initializer.get());
	}
	return std::make_shared<ast::VarDecl>(identifier, nullptr);
}

ast::ProgramPtr make_program(const std::vector<ast::StatementPtr> &statements)
{
	return std::make_shared<ast::Program>(statements);
}

ast::CallPtr make_call(const ast::ExpressionPtr &callee, boost::optional<std::vector<ast::ExpressionPtr>> args)
{
	auto call = std::make_shared<ast::Call>(callee);

	if (args)
	{
		call->SetArguments(args.get());
	}

	return call;
}

ast::GetExpressionPtr make_get_expression(const ast::ExpressionPtr &lhs, const ast::IdentifierPtr &identifier)
{
	return std::make_shared<ast::GetExpression>(lhs, identifier);
}

ast::SetExpressionPtr make_setter(const ast::ExpressionPtr &lhs, const ast::ExpressionPtr &rhs)
{
	auto getter = std::dynamic_pointer_cast<ast::GetExpression>(lhs);
	return std::make_shared<ast::SetExpression>(getter->GetLhs(), getter->GetIdentifier(), rhs);
}

ast::FunctionDeclarationPtr make_function(ast::IdentifierPtr identifier,
                                          boost::optional<std::vector<ast::ParameterPtr>> &parameters,
                                          ast::StatementBlockPtr body)
{
	if (parameters)
	{
		return std::make_shared<ast::FunctionDeclaration>(identifier, parameters.get(), body);
	}
	return std::make_shared<ast::FunctionDeclaration>(identifier, std::vector<ast::ParameterPtr>{}, body);
}

ast::ParameterPtr make_parameter(ast::IdentifierPtr identifier)
{
	return std::make_shared<ast::Parameter>(identifier->GetIdentifier());
}

ast::ParameterPtr add_parameter_type(ast::ParameterPtr parameter, ast::IdentifierPtr parameterType)
{
	parameter->SetParameterType(parameterType->GetIdentifier());
	return parameter;
}

ast::ReturnPtr make_return_statement(boost::optional<ast::ExpressionPtr> return_expression)
{
	if (return_expression)
	{
		return std::make_shared<ast::Return>(return_expression.get());
	}
	return std::make_shared<ast::Return>(nullptr);
}

ast::AnonymousMethodPtr make_anonymous_method(const ast::ExpressionPtr &instance, const ast::StatementBlockPtr &body)
{
	return std::make_shared<ast::AnonymousMethod>(instance, body);
}

ast::ClassDeclarationPtr make_class_declaration(ast::IdentifierPtr identifier,
                                                std::vector<ast::FunctionDeclarationPtr> methods)
{
	return std::make_shared<ast::ClassDeclaration>(identifier, methods);
}

ast::StatementPtr make_for_loop(
    const boost::optional<boost::variant<ast::VarDeclPtr, ast::ExpressionStatementPtr>> &init,
    const boost::optional<ast::ExpressionPtr> &condition, const boost::optional<ast::ExpressionPtr> &increment,
    const ast::StatementPtr body)
{
	auto outerBlock = std::make_shared<ast::StatementBlock>();

	if (init)
	{
		struct get_init : public boost::static_visitor<ast::StatementPtr>
		{
			ast::StatementPtr operator()(const ast::VarDeclPtr &v) const { return v; }
			ast::StatementPtr operator()(const ast::ExpressionStatementPtr &e) const { return e; }
		};

		get_init i;
		outerBlock->AddStatement(boost::apply_visitor(i, init.get()));
	}

	ast::LoopPtr loop;
	auto innerBlock = std::make_shared<ast::StatementBlock>();
	innerBlock->AddStatement(body);

	if (increment)
	{
		innerBlock->AddStatement(make_expression_statment(increment.get()));
	}

	if (condition)
	{
		loop = std::make_shared<ast::Loop>(condition.get(), innerBlock);
	}
	else
	{
		loop = std::make_shared<ast::Loop>(make_true(), innerBlock);
	}

	outerBlock->AddStatement(loop);

	return outerBlock;
}

}  // namespace pagoda
