#include "interpreter_visitor.h"

#include "../../dynamic_value/binding/binary_ops.h"
#include "../../dynamic_value/binding/make_free_function.h"
#include "../../dynamic_value/binding/unary_ops.h"
#include "../../dynamic_value/boolean_value.h"
#include "../../dynamic_value/class_base.h"
#include "../../dynamic_value/dynamic_class.h"
#include "../../dynamic_value/dynamic_instance.h"
#include "../../dynamic_value/dynamic_value_base.h"
#include "../../dynamic_value/dynamic_value_table.h"
#include "../../dynamic_value/float_value.h"
#include "../../dynamic_value/function.h"
#include "../../dynamic_value/get_value_as.h"
#include "../../dynamic_value/icallable.h"
#include "../../dynamic_value/integer_value.h"
#include "../../dynamic_value/null_object_value.h"
#include "../../dynamic_value/string_value.h"
#include "../../dynamic_value/type_info.h"
#include "../../dynamic_value/value_visitor.h"
#include "../../dynamic_value/vector3.h"
#include "../value/builtin_functions.h"

#include "../value/script_callable_body.h"

#include <pagoda/common/exception/exception.h>

namespace pagoda
{
// clang-format off
#define MAKE_BUILTIN_CALLABLE(function) { #function, pagoda::make_free_function( #function , make_function(function)) }
// clang-format on

interpreter_visitor::interpreter_visitor()
    : m_globals(std::make_shared<DynamicValueTable>("Global")), m_symbolTable(m_globals)
{
}

interpreter_visitor::~interpreter_visitor() {}

void interpreter_visitor::Visit(ast::FloatPtr n) { PushValue(std::make_shared<FloatValue>(n->GetNumber())); }

void interpreter_visitor::Visit(ast::IntegerPtr n) { PushValue(std::make_shared<Integer>(n->GetInteger())); }

void interpreter_visitor::Visit(ast::StringPtr s) { PushValue(std::make_shared<String>(s->GetString())); }

void interpreter_visitor::Visit(ast::IdentifierPtr i) { PushValue(GetCurrentSymbolTable()->Get(i->GetIdentifier())); }

void interpreter_visitor::Visit(ast::BooleanPtr b) { PushValue(std::make_shared<Boolean>(b->GetBoolean())); }

void interpreter_visitor::Visit(ast::Nullptr n) { PushValue(std::make_shared<NullObject>()); }

void interpreter_visitor::Visit(ast::ArithmeticOpPtr op)
{
	op->GetRhs()->AcceptVisitor(this);
	op->GetLhs()->AcceptVisitor(this);

	auto lhs = PopValue();
	auto rhs = PopValue();

	switch (op->GetOperationType())
	{
		case ast::ArithmeticOp::types::Add:
		{
			binary_op_dispatcher<add> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
		case ast::ArithmeticOp::types::Sub:
		{
			binary_op_dispatcher<sub> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
		case ast::ArithmeticOp::types::Mul:
		{
			binary_op_dispatcher<mul> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
		case ast::ArithmeticOp::types::Div:
		{
			binary_op_dispatcher<div> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
	}
}

void interpreter_visitor::Visit(ast::UnaryPtr u)
{
	u->GetRhs()->AcceptVisitor(this);
	auto rhs = PopValue();

	switch (u->GetOperationType())
	{
		case ast::Unary::types::Neg:
		{
			unary_ops_dispatcher<negate> v;
			PushValue(apply_visitor(v, *rhs));
			break;
		}
		case ast::Unary::types::Min:
		{
			unary_ops_dispatcher<minus> v;
			PushValue(apply_visitor(v, *rhs));
			break;
		}
	}
}

void interpreter_visitor::Visit(ast::ComparisonOpPtr op)
{
	op->GetRhs()->AcceptVisitor(this);
	op->GetLhs()->AcceptVisitor(this);
	auto lhs = PopValue();
	auto rhs = PopValue();

	switch (op->GetOperationType())
	{
		case ast::ComparisonOp::types::Eq:
		{
			binary_op_dispatcher<eq> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
		case ast::ComparisonOp::types::Ne:
		{
			binary_op_dispatcher<neq> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
		case ast::ComparisonOp::types::Gt:
		{
			binary_op_dispatcher<gt> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
		case ast::ComparisonOp::types::Gte:
		{
			binary_op_dispatcher<gte> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
		case ast::ComparisonOp::types::Lt:
		{
			binary_op_dispatcher<lt> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
		case ast::ComparisonOp::types::Lte:
		{
			binary_op_dispatcher<lte> v(lhs, rhs);
			PushValue(apply_visitor(v, *lhs));
			break;
		}
	}
}

void interpreter_visitor::Visit(ast::LogicOpPtr op)
{
	op->GetLhs()->AcceptVisitor(this);

	auto lhs = PopValue();

	auto lhs_true = get_value_as<bool>(*lhs);
	switch (op->GetOperationType())
	{
		case ast::LogicOp::types::And:
		{
			if (lhs_true)
			{
				op->GetRhs()->AcceptVisitor(this);
				auto rhs = PopValue();
				PushValue(std::make_shared<Boolean>(get_value_as<bool>(*rhs)));
				return;
			}
			PushValue(std::make_shared<Boolean>(false));
			break;
		}
		case ast::LogicOp::types::Or:
		{
			if (lhs_true)
			{
				PushValue(std::make_shared<Boolean>(true));
				return;
			}
			op->GetRhs()->AcceptVisitor(this);
			auto rhs = PopValue();
			PushValue(std::make_shared<Boolean>(get_value_as<bool>(*rhs)));
			break;
		}
	}
}

void interpreter_visitor::Visit(ast::AssignmentPtr a)
{
	a->GetRhs()->AcceptVisitor(this);
	auto rhs = PopValue();
	GetCurrentSymbolTable()->Assign(a->GetIdentifier()->GetIdentifier(), rhs);
	PushValue(rhs);
}

void interpreter_visitor::Visit(ast::ExpressionStatementPtr e)
{
	e->GetExpression()->AcceptVisitor(this);
	PopValue();
}

void interpreter_visitor::Visit(ast::IfStatementPtr i)
{
	i->GetCondition()->AcceptVisitor(this);
	auto condition = PopValue();

	auto falseStatement = i->GetFalseStatement();

	if (get_value_as<bool>(*condition))
	{
		i->GetTrueStatement()->AcceptVisitor(this);
	}
	else if (falseStatement)
	{
		falseStatement->AcceptVisitor(this);
	}
}

void interpreter_visitor::Visit(ast::LoopPtr l)
{
	while (true)
	{
		l->GetCondition()->AcceptVisitor(this);
		auto condition = PopValue();
		if (!get_value_as<bool>(*condition))
		{
			break;
		}
		l->GetBody()->AcceptVisitor(this);
	}
}

void interpreter_visitor::Visit(ast::VarDeclPtr v)
{
	auto rhs = v->GetRhs();

	if (rhs)
	{
		rhs->AcceptVisitor(this);
		GetCurrentSymbolTable()->Declare(v->GetIdentifier()->GetIdentifier(), PopValue());
	}
	else
	{
		GetCurrentSymbolTable()->Declare(v->GetIdentifier()->GetIdentifier(), std::make_shared<NullObject>());
	}
}

void interpreter_visitor::Visit(ast::StatementBlockPtr b)
{
	auto prevSymbolTable = GetCurrentSymbolTable();
	EnterBlock();

	for (const auto &statement : b->GetStatements())
	{
		statement->AcceptVisitor(this);
	}

	ExitBlock(prevSymbolTable);
}

void interpreter_visitor::EnterBlock()
{
	m_symbolTable = std::make_shared<DynamicValueTable>("Block", GetCurrentSymbolTable());
}

void interpreter_visitor::ExitBlock(const std::shared_ptr<DynamicValueTable> &previousSymbolTable)
{
	m_symbolTable = previousSymbolTable;
}

void interpreter_visitor::EnterFunction(const ICallablePtr &callable)
{
	auto closure = callable->GetClosure();
	if (closure)
	{
		m_symbolTable = std::make_shared<DynamicValueTable>(callable->GetCallableName(), closure);
	}
	else
	{
		m_symbolTable = std::make_shared<DynamicValueTable>(callable->GetCallableName(), m_globals);
	}
}

void interpreter_visitor::ExitFunction(const std::shared_ptr<DynamicValueTable> &previousSymbolTable)
{
	m_symbolTable = previousSymbolTable;
}

void interpreter_visitor::Visit(ast::CallPtr c)
{
	c->GetCallee()->AcceptVisitor(this);
	auto popped = PopValue();
	auto callee = std::dynamic_pointer_cast<ICallable>(popped);

	std::vector<DynamicValueBasePtr> args;
	for (auto &a : c->GetArguments())
	{
		a->AcceptVisitor(this);
		args.push_back(PopValue());
	}

	if (!callee->IsVariadic() && callee->GetArity() != args.size())
	{
		throw common::exception::Exception("Wrong arity");
	}

	auto prevSymbolTable = GetCurrentSymbolTable();
	EnterFunction(callee);
	PushValue(callee->Call(args));
	ExitFunction(prevSymbolTable);
}

void interpreter_visitor::Visit(ast::FunctionDeclarationPtr func)
{
	auto identifier = func->GetIdentifier();
	auto parameters = func->GetParameters();
	auto body = func->GetFunctionBody();
	auto callableBody = std::make_shared<ScriptCallableBody>(identifier, body, parameters);

	auto callable = std::make_shared<Function>(callableBody);
	callable->SetCallableName(identifier->GetIdentifier());
	callable->SetClosure(GetCurrentSymbolTable());

	/*
	callable->SetParameters(parameters);
	*/
	callable->SetArity(parameters.size());

	GetCurrentSymbolTable()->Declare(identifier->GetIdentifier(), callable);
}

void interpreter_visitor::Visit(ast::ParameterPtr par) { throw common::exception::Exception("Unimplemented"); }

void interpreter_visitor::Visit(ast::ReturnPtr r)
{
	auto returnExression = r->GetReturnExpression();
	if (returnExression)
	{
		returnExression->AcceptVisitor(this);
		throw static_cast<DynamicValueBasePtr>(PopValue());
	}

	throw static_cast<DynamicValueBasePtr>(std::make_shared<NullObject>());
}

void interpreter_visitor::Visit(ast::ClassDeclarationPtr c)
{
	auto &identifier = c->GetIdentifier();
	auto klass = std::make_shared<DynamicClass>(identifier->GetIdentifier());

	GetCurrentSymbolTable()->Declare(identifier->GetIdentifier(), klass);

	for (auto &m : c->GetMethods())
	{
		auto identifier = m->GetIdentifier();
		auto parameters = m->GetParameters();
		auto body = m->GetFunctionBody();
		auto callableBody = std::make_shared<ScriptCallableBody>(identifier, body, parameters);

		auto callable = std::make_shared<Function>(callableBody);
		callable->SetClosure(GetCurrentSymbolTable());
		callable->SetArity(parameters.size());

		// klass->AddMethod(m->GetIdentifier()->GetIdentifier(), callable);
	}
}

void interpreter_visitor::Visit(ast::AnonymousMethodPtr a)
{
	/*
	a->GetInstance()->AcceptVisitor(this);
	auto instance = std::dynamic_pointer_cast<Instance>(PopValue());

	auto body = a->GetBody();
	auto callableBody = std::make_shared<ScriptCallableBody>(nullptr, body, {});

	auto callable = std::make_shared<Function>(callableBody);
	callable->SetCallableName("AnonymousMethod");
	callable->SetClosure(GetCurrentSymbolTable());
	callable->SetArity(0);

	callable = instance->Bind(callable, m_globals);
	auto prevSymbolTable = GetCurrentSymbolTable();

	EnterFunction(callable);
	std::vector<DynamicValueBasePtr> args;
	try
	{
	    callable->Call(this, args);
	}
	catch (DynamicValueBasePtr &f)
	{
	    ExitFunction(prevSymbolTable);
	    PushValue(f);
	    return;
	}
	ExitFunction(prevSymbolTable);

	// By default an anonymous method returns the instance
	PushValue(instance);
	*/
}

void interpreter_visitor::Visit(ast::GetExpressionPtr e)
{
	e->GetLhs()->AcceptVisitor(this);
	auto lhs = std::dynamic_pointer_cast<ClassBase>(PopValue());
	auto identifier = e->GetIdentifier()->GetIdentifier();

	auto value = lhs->GetMember(identifier);
	auto method = std::dynamic_pointer_cast<Function>(value);
	if (method)
	{
		PushValue(lhs->Bind(method->GetCallableBody(), m_globals));
	}
	else
	{
		PushValue(value);
	}
}

void interpreter_visitor::Visit(ast::SetExpressionPtr e)
{
	e->GetLhs()->AcceptVisitor(this);
	auto lhs = std::dynamic_pointer_cast<ClassBase>(PopValue());
	auto identifier = e->GetIdentifier()->GetIdentifier();
	e->GetRhs()->AcceptVisitor(this);
	auto rhs = PopValue();

	lhs->GetInstanceValueTable()->Declare(identifier, rhs);
	PushValue(rhs);
}

void interpreter_visitor::Visit(ast::ProgramPtr p)
{
	for (const auto &statement : p->GetStatements())
	{
		statement->AcceptVisitor(this);
	}
}

void interpreter_visitor::PushValue(const DynamicValueBasePtr &v) { m_values.push(v); }

DynamicValueBasePtr interpreter_visitor::PopValue()
{
	m_lastValue = m_values.top();
	m_values.pop();
	return m_lastValue;
}
}  // namespace pagoda
