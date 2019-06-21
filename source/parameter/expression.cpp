#include "expression.h"

#include <intermediate/ast.h>
#include <intermediate/ast_visitor.h>
#include <interpreter/interpreter.h>
#include <parser/parser.h>
#include <value/float_value.h>
#include <value/integer_value.h>
#include <value/string_value.h>
#include <value/value_visitor.h>

#include "common/profiler.h"
#include "parameter_exception.h"

using namespace sscript;

namespace selector
{
class ExpressionInterpreter : public Interpreter
{
public:
	static ExpressionInterpreter& GetInstance()
	{
		static ExpressionInterpreter sInterpreter;
		return sInterpreter;
	}

private:
	ExpressionInterpreter() : Interpreter()
	{
		// TODO: Add built-in functions
	}
};

class ExpressionValidator : public AstVisitor
{
public:
	void Visit(ast::FloatPtr) override {}

	void Visit(ast::IntegerPtr) override {}

	void Visit(ast::StringPtr) override {}

	void Visit(ast::IdentifierPtr i) override { m_symbols.push_back(i->GetIdentifier()); }

	void Visit(ast::BooleanPtr) override {}

	void Visit(ast::Nullptr) override {}

	void Visit(ast::ArithmeticOpPtr a) override
	{
		a->GetLhs()->AcceptVisitor(this);
		a->GetRhs()->AcceptVisitor(this);
	}

	void Visit(ast::UnaryPtr u) override { u->GetRhs()->AcceptVisitor(this); }

	void Visit(ast::ComparisonOpPtr c) override
	{
		c->GetLhs()->AcceptVisitor(this);
		c->GetRhs()->AcceptVisitor(this);
	}

	void Visit(ast::LogicOpPtr l) override
	{
		l->GetLhs()->AcceptVisitor(this);
		l->GetRhs()->AcceptVisitor(this);
	}

	void Visit(ast::AssignmentPtr a) override
	{
		a->GetIdentifier()->AcceptVisitor(this);
		a->GetRhs()->AcceptVisitor(this);
	}

	void Visit(ast::ExpressionStatementPtr e) override { e->GetExpression()->AcceptVisitor(this); }

	void Visit(ast::IfStatementPtr i) override
	{
		i->GetCondition()->AcceptVisitor(this);
		i->GetTrueStatement()->AcceptVisitor(this);
		auto falseStatement = i->GetFalseStatement();
		if (falseStatement)
		{
			falseStatement->AcceptVisitor(this);
		}
	}

	void Visit(ast::LoopPtr l) override
	{
		l->GetCondition()->AcceptVisitor(this);
		l->GetBody()->AcceptVisitor(this);
	}

	void Visit(ast::VarDeclPtr v) override
	{
		v->GetIdentifier()->AcceptVisitor(this);
		auto rhs = v->GetRhs();
		if (rhs)
		{
			rhs->AcceptVisitor(this);
		}
	}

	void Visit(ast::StatementBlockPtr s) override
	{
		for (auto& s : s->GetStatements())
		{
			s->AcceptVisitor(this);
		}
	}

	void Visit(ast::CallPtr c) override
	{
		c->GetCallee()->AcceptVisitor(this);
		for (auto& a : c->GetArguments())
		{
			a->AcceptVisitor(this);
		}
	}

	void Visit(ast::FunctionDeclarationPtr f) override {}

	void Visit(ast::ClassDeclarationPtr) override {}

	void Visit(ast::AnonymousMethodPtr) override {}

	void Visit(ast::GetExpressionPtr g) override
	{
		g->GetLhs()->AcceptVisitor(this);
		g->GetIdentifier()->AcceptVisitor(this);
	}

	void Visit(ast::SetExpressionPtr s) override
	{
		s->GetLhs()->AcceptVisitor(this);
		s->GetIdentifier()->AcceptVisitor(this);
	}

	void Visit(ast::ReturnPtr) override {}

	void Visit(ast::ProgramPtr p) override
	{
		for (auto& s : p->GetStatements())
		{
			s->AcceptVisitor(this);
		}
	}

	void Visit(ast::ParameterPtr p) override {}

	std::vector<std::string> m_symbols;
};

struct to_float : public ValueVisitor<float>
{
	float operator()(Float& f) { return f.m_value; }
	float operator()(Integer& i) { return i.m_value; }

	template<typename V>
	float operator()(V&)
	{
		throw ParameterException("Unable to evaluate expression to float.");
	}
};

struct to_string : public ValueVisitor<std::string>
{
	std::string operator()(String& s) { return s.m_value; }

	template<typename V>
	std::string operator()(V&)
	{
		throw ParameterException("Unable to evaluate expression to string.");
	}
};

class Expression::Impl
{
public:
	Impl(ast::ProgramPtr&& program) : m_expression(program) {}
	~Impl() {}

	float GetAsFloat() const
	{
		auto& i = ExpressionInterpreter::GetInstance();
		i.Interpret(m_expression);
		auto lastValue = i.GetLastEvaluatedExpression();
		to_float f;

		return apply_visitor(f, lastValue);
	}

	std::string GetAsString() const
	{
		auto& i = ExpressionInterpreter::GetInstance();
		i.Interpret(m_expression);
		auto lastValue = i.GetLastEvaluatedExpression();
		to_string s;

		return apply_visitor(s, lastValue);
	}

	ast::ProgramPtr m_expression;
	std::vector<std::string> m_variables;
};

std::shared_ptr<Expression> Expression::CreateExpression(const std::string& expressionString)
{
	auto expression = std::make_shared<Expression>();
	Parser p;
	expression->m_implementation = std::make_unique<Expression::Impl>(p.Parse(expressionString));

	ExpressionValidator validator;
	expression->m_implementation->m_expression->AcceptVisitor(&validator);
	expression->m_implementation->m_variables = validator.m_symbols;

	return expression;
}

Expression::Expression() : m_implementation(nullptr) {}

const std::vector<std::string>& Expression::GetVariables() const { return m_implementation->m_variables; }

float Expression::GetAsFloat() const { return m_implementation->GetAsFloat(); }

std::string Expression::GetAsString() const { return m_implementation->GetAsString(); }

}  // namespace selector

