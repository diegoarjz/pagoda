#include "expression.h"

#include "boolean_value.h"
#include "dynamic_class.h"
#include "dynamic_instance.h"
#include "dynamic_value_table.h"
#include "float_value.h"
#include "function.h"
#include "integer_value.h"
#include "null_object_value.h"
#include "string_value.h"
#include "value_not_found.h"
#include "value_visitor.h"

#include <pagoda/pgscript/interpreter/interpreter.h>
#include <pagoda/pgscript/ir/ast.h>
#include <pagoda/pgscript/ir/ast_visitor.h>
#include <pagoda/pgscript/parser/parser.h>

#include <pagoda/common/instrument/profiler.h>

#include <unordered_map>

namespace pagoda::dynamic
{
const TypeInfoPtr Expression::s_typeInfo = std::make_shared<TypeInfo>("Expression");

class ExpressionInterpreter : public Interpreter
{
	public:
	static ExpressionInterpreter &GetInstance()
	{
		static ExpressionInterpreter sInterpreter;
		return sInterpreter;
	}

	static std::shared_ptr<DynamicInstance> MakeParameterInstance()
	{
		return std::make_shared<DynamicInstance>(m_parameterClass);
	}

	private:
	ExpressionInterpreter() : Interpreter()
	{
		// TODO: Add built-in functions
	}

	static const DynamicClassPtr m_parameterClass;
};

const DynamicClassPtr ExpressionInterpreter::m_parameterClass = std::make_shared<DynamicClass>("PagodaObject");

class ExpressionValidator : public AstVisitor
{
	public:
	ExpressionValidator() : m_getExpressionCount(0) {}

	void Visit(ast::FloatPtr) override {}

	void Visit(ast::IntegerPtr) override {}

	void Visit(ast::StringPtr) override {}

	void Visit(ast::IdentifierPtr i) override
	{
		if (m_getExpressionCount == 0) {
			m_symbols.push_back(Variable(i->GetIdentifier()));
		} else {
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
		if (falseStatement) {
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
		if (rhs) {
			rhs->AcceptVisitor(this);
		}
	}

	void Visit(ast::StatementBlockPtr s) override
	{
		for (auto &s : s->GetStatements()) {
			s->AcceptVisitor(this);
		}
	}

	void Visit(ast::CallPtr c) override
	{
		c->GetCallee()->AcceptVisitor(this);
		for (auto &a : c->GetArguments()) {
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
		for (auto &s : p->GetStatements()) {
			s->AcceptVisitor(this);
		}
	}

	void Visit(ast::ParameterPtr p) override {}

	std::vector<Variable> m_symbols;
	uint32_t m_getExpressionCount;
};

struct dependent_expression_adder : public ValueVisitor<void>
{
	dependent_expression_adder(ExpressionPtr expression) : m_expression(expression) {}

	void operator()(Expression &e) { e.AddDependentExpression(m_expression); }

	template<typename T>
	void operator()(const T &)
	{
	}

	ExpressionPtr m_expression;
};

class Expression::Impl : public std::enable_shared_from_this<Expression::Impl>
{
	public:
	Impl(ast::ProgramPtr &&program) : m_expression(program) {}
	~Impl() {}

	DynamicValueBasePtr Evaluate()
	{
		START_PROFILE;

		if (m_lastComputedValue == nullptr) {
			auto &interpreter = ExpressionInterpreter::GetInstance();

			auto variables = std::make_shared<DynamicValueTable>("variables");
			for (auto &var : m_variableValues) {
				const std::list<std::string> &variableIdentifiers = var.first.GetIdentifiers();
				variables->Declare(variableIdentifiers.front(), EvaluateIfExpression(var.second));
			}
			interpreter.PushExternalSymbols(variables);

			interpreter.Interpret(m_expression);
			m_lastComputedValue = interpreter.GetLastEvaluatedExpression();

			// TODO: this would not be needed if ExpressionInterpreter wasn't a singleton
			interpreter.PopExternalSymbols();
		}

		return m_lastComputedValue;
	}

	DynamicValueBasePtr EvaluateIfExpression(const DynamicValueBasePtr &d)
	{
		ExpressionPtr e = std::dynamic_pointer_cast<Expression>(d);
		if (e != nullptr) {
			return EvaluateIfExpression(e->Evaluate());
		}
		return d;
	}

	void SetVariableValue(const Variable &variableName, DynamicValueBasePtr value)
	{
		START_PROFILE;

		dependent_expression_adder adder(m_expressionInterface);
		apply_visitor(adder, *value);

		m_variableValues[variableName] = value;
		SetDirty();
	}

	void SetVariableValue(const std::string &variableName, DynamicValueBasePtr value)
	{
		SetVariableValue(Variable(variableName), value);
	}

	void AddDependentExpression(ExpressionPtr e) { m_dependentExpressions.push_back(e); }

	const std::vector<std::weak_ptr<Expression>> GetDependentExpressions() const { return m_dependentExpressions; }

	void SetDirty()
	{
		m_lastComputedValue = nullptr;
		for (const auto &e : m_dependentExpressions) {
			e.lock()->SetDirty();
		}
	}

	bool IsDirty() const { return m_lastComputedValue == nullptr; }

	std::string ToString() const { return "<Expression>"; }

	ast::ProgramPtr m_expression;
	std::unordered_set<Variable, Variable::Hash> m_variables;
	std::unordered_map<Variable, DynamicValueBasePtr, Variable::Hash> m_variableValues;
	std::vector<std::weak_ptr<Expression>> m_dependentExpressions;
	DynamicValueBasePtr m_lastComputedValue;
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

	for (const auto &v : validator.m_symbols) {
		expression->m_implementation->m_variables.insert(v);
	}

	return expression;
}

Expression::Expression() : DynamicValueBase(s_typeInfo), m_implementation(nullptr) {}

const std::unordered_set<Variable, Variable::Hash> &Expression::GetVariables() const
{
	return m_implementation->m_variables;
}

void Expression::SetVariableValue(const Variable &variableName, DynamicValueBasePtr value)
{
	m_implementation->SetVariableValue(variableName, value);
}

void Expression::SetVariableValue(const std::string &variableName, DynamicValueBasePtr value)
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

std::string Expression::ToString() const { return m_implementation->ToString(); }

void Expression::AcceptVisitor(ValueVisitorBase &visitor) { visitor.Visit(*this); }

DynamicValueBasePtr Expression::Evaluate() { return m_implementation->Evaluate(); }
}  // namespace pagoda::dynamic
