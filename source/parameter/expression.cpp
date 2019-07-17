#include "expression.h"

#include <intermediate/ast.h>
#include <intermediate/ast_visitor.h>
#include <interpreter/interpreter.h>
#include <interpreter/symbol_table.h>
#include <parser/parser.h>
#include <value/float_value.h>
#include <value/integer_value.h>
#include <value/string_value.h>
#include <value/value_visitor.h>

#include "common/profiler.h"
#include "parameter_exception.h"

#include <unordered_map>

using namespace sscript;

namespace selector
{
class ExpressionInterpreter : public Interpreter
{
public:
	static ExpressionInterpreter &GetInstance()
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
	ExpressionValidator() : m_getExpressionCount(0) {}

	void Visit(ast::FloatPtr) override {}

	void Visit(ast::IntegerPtr) override {}

	void Visit(ast::StringPtr) override {}

	void Visit(ast::IdentifierPtr i) override
	{
		if (m_getExpressionCount == 0)
		{
			m_symbols.push_back(Variable(i->GetIdentifier()));
		}
		else
		{
			m_symbols.back().AddIdentifier(i->GetIdentifier());
		}
	}

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
		for (auto &s : s->GetStatements())
		{
			s->AcceptVisitor(this);
		}
	}

	void Visit(ast::CallPtr c) override
	{
		c->GetCallee()->AcceptVisitor(this);
		for (auto &a : c->GetArguments())
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
		++m_getExpressionCount;
		g->GetIdentifier()->AcceptVisitor(this);
		--m_getExpressionCount;
	}

	void Visit(ast::SetExpressionPtr s) override
	{
		s->GetLhs()->AcceptVisitor(this);
		s->GetIdentifier()->AcceptVisitor(this);
	}

	void Visit(ast::ReturnPtr) override {}

	void Visit(ast::ProgramPtr p) override
	{
		for (auto &s : p->GetStatements())
		{
			s->AcceptVisitor(this);
		}
	}

	void Visit(ast::ParameterPtr p) override {}

	std::vector<Variable> m_symbols;
	uint32_t m_getExpressionCount;
};

struct to_float : public ValueVisitor<float>
{
	float operator()(Float &f) { return f.m_value; }
	float operator()(Integer &i) { return i.m_value; }

	template<typename V>
	float operator()(V &)
	{
		throw ParameterException("Unable to evaluate expression to float.");
	}
};

struct to_string : public ValueVisitor<std::string>
{
	std::string operator()(String &s) { return s.m_value; }

	template<typename V>
	std::string operator()(V &)
	{
		throw ParameterException("Unable to evaluate expression to string.");
	}
};

struct base_value_to_parameter : public ValueVisitor<Parameter>
{
	Parameter operator()(Float &f) { return f.m_value; }
	Parameter operator()(String &s) { return s.m_value; }

	template<typename V>
	std::string operator()(V &)
	{
		throw ParameterException("Unimplemented Expression to Parameter conversion");
	}
};

struct parameter_to_base_value
{
	BaseValuePtr operator()(const float &f) { return std::make_shared<Float>(f); }
	BaseValuePtr operator()(const std::string &s) { return std::make_shared<String>(s); }
	BaseValuePtr operator()(const ExpressionPtr &e)
	{
		auto parameter = e->GetAsParameter();
		return std::visit(*this, parameter);
	}
};

class Expression::Impl : public std::enable_shared_from_this<Expression::Impl>
{
public:
	Impl(ast::ProgramPtr &&program) : m_expression(program) {}
	~Impl() {}

	float GetAsFloat() { return GetAs<float, to_float>(); }

	std::string GetAsString() { return GetAs<std::string, to_string>(); }

	Parameter GetAsParameter() { return GetAs<Parameter, base_value_to_parameter>(); }

	template<class Return_t, class Visitor_t>
	Return_t GetAs()
	{
		Evaluate();
		Visitor_t visitor;
		return apply_visitor(visitor, m_lastComputedValue);
	}

	void Evaluate()
	{
		START_PROFILE;

		if (m_lastComputedValue == nullptr)
		{
			auto &interpreter = ExpressionInterpreter::GetInstance();

			auto variables = std::make_shared<SymbolTable>("variables");
			parameter_to_base_value converter;
			for (const auto &var : m_variableValues)
			{
				// TODO: This needs fixing if we're going to have compound variables here.
				variables->Declare({var.first.GetIdentifiers().front(), std::visit(converter, var.second)});
			}
			interpreter.PushExternalSymbols(variables);

			interpreter.Interpret(m_expression);
			m_lastComputedValue = interpreter.GetLastEvaluatedExpression();

			// TODO: this would not be needed if ExpressionInterpreter wasn't a singleton
			interpreter.PopExternalSymbols();
		}
	}

	void SetVariableValue(const Variable &variableName, Parameter value)
	{
		START_PROFILE;

		struct dependent_expression_adder
		{
			dependent_expression_adder(ExpressionPtr expression) : m_expression(expression) {}

			void operator()(const ExpressionPtr &e) { e->AddDependentExpression(m_expression); }
			void operator()(const float &) {}
			void operator()(const std::string &) {}

			ExpressionPtr m_expression;
		};

		dependent_expression_adder adder(m_expressionInterface);
		std::visit(adder, value);

		m_variableValues[variableName] = value;
		SetDirty();
	}

	void SetVariableValue(const std::string &variableName, Parameter value)
	{
		SetVariableValue(Variable(variableName), value);
	}

	void AddDependentExpression(ExpressionPtr e) { m_dependentExpressions.push_back(e); }

	const std::vector<std::weak_ptr<Expression>> GetDependentExpressions() const { return m_dependentExpressions; }

	void SetDirty()
	{
		m_lastComputedValue = nullptr;
		for (const auto &e : m_dependentExpressions)
		{
			e.lock()->SetDirty();
		}
	}

	bool IsDirty() const { return m_lastComputedValue == nullptr; }

	ast::ProgramPtr m_expression;
	std::unordered_set<Variable, Variable::Hash> m_variables;
	std::unordered_map<Variable, Parameter, Variable::Hash> m_variableValues;
	std::vector<std::weak_ptr<Expression>> m_dependentExpressions;
	BaseValuePtr m_lastComputedValue;
	ExpressionPtr m_expressionInterface;
};

std::shared_ptr<Expression> Expression::CreateExpression(const std::string &expressionString)
{
	START_PROFILE;

	auto expression = std::make_shared<Expression>();
	Parser p;
	expression->m_implementation = std::make_unique<Expression::Impl>(p.Parse(expressionString));
	expression->m_implementation->m_expressionInterface = expression;

	ExpressionValidator validator;
	expression->m_implementation->m_expression->AcceptVisitor(&validator);
	std::copy(
	    validator.m_symbols.begin(), validator.m_symbols.end(),
	    std::inserter(expression->m_implementation->m_variables, expression->m_implementation->m_variables.end()));

	return expression;
}

Expression::Expression() : m_implementation(nullptr) {}

const std::unordered_set<Variable, Variable::Hash> &Expression::GetVariables() const
{
	return m_implementation->m_variables;
}

void Expression::SetVariableValue(const Variable &variableName, Parameter value)
{
	m_implementation->SetVariableValue(variableName, value);
}

void Expression::SetVariableValue(const std::string &variableName, Parameter value)
{
	m_implementation->SetVariableValue(variableName, value);
}

void Expression::SetDirty() { m_implementation->SetDirty(); }

bool Expression::IsDirty() const { return m_implementation->IsDirty(); }

void Expression::AddDependentExpression(ExpressionPtr e) { m_implementation->AddDependentExpression(e); }

const std::vector<std::weak_ptr<Expression>> Expression::GetDependentExpressions() const
{
	return m_implementation->GetDependentExpressions();
}

Parameter Expression::GetAsParameter() { return m_implementation->GetAsParameter(); }

float Expression::GetAsFloat() { return m_implementation->GetAsFloat(); }

std::string Expression::GetAsString() { return m_implementation->GetAsString(); }

}  // namespace selector

