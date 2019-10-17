#include "interpreter_visitor.h"

#include "../binding/make_builtin_function.h"
#include "../value/base_value.h"
#include "../value/boolean_value.h"
#include "../value/builtin_function.h"
#include "../value/builtin_functions.h"
#include "../value/callable.h"
#include "../value/class_value.h"
#include "../value/float_value.h"
#include "../value/function.h"
#include "../value/instance_value.h"
#include "../value/integer_value.h"
#include "../value/null_object_value.h"
#include "../value/string_value.h"
#include "../value/value_visitor.h"

#include "symbol_table.h"

namespace sscript
{
namespace bin_ops
{
struct add
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs + rhs;
	}
};

struct sub
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs - rhs;
	}
};

struct mul
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs * rhs;
	}
};

struct div
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs / rhs;
	}
};

struct eq
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs == rhs;
	}
};

struct ne
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs != rhs;
	}
};

struct gt
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs > rhs;
	}
};

struct gte
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs >= rhs;
	}
};

struct lt
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs < rhs;
	}
};

struct lte
{
	template<typename LHS, typename RHS>
	static BaseValuePtr apply(const LHS &lhs, const RHS &rhs)
	{
		return lhs <= rhs;
	}
};
}  // namespace bin_ops

namespace unary_ops
{
struct negate
{
	template<typename T>
	static BaseValuePtr apply(const T &v)
	{
		return std::make_shared<Boolean>(!v);
	}
};

struct minus
{
	template<typename T>
	static BaseValuePtr apply(const T &v)
	{
		return -v;
	}
};
}  // namespace unary_ops

template<class Op>
struct unary_ops_dispatcher : public ValueVisitor<BaseValuePtr>
{
	template<typename Operand>
	BaseValuePtr operator()(const Operand &operand)
	{
		return Op::apply(operand);
	}
};

template<class OP>
struct binary_op_dispatcher : public ValueVisitor<BaseValuePtr>
{
	BaseValuePtr m_lhs;
	BaseValuePtr m_rhs;

	binary_op_dispatcher(const BaseValuePtr &lhs, const BaseValuePtr &rhs) : m_lhs(lhs), m_rhs(rhs) {}

	template<typename LHS>
	struct binary_op_rhs_dispatcher : public ValueVisitor<BaseValuePtr>
	{
		const LHS &m_lhs;
		explicit binary_op_rhs_dispatcher(const LHS &lhs) : m_lhs(lhs) {}

		template<typename RHS>
		BaseValuePtr operator()(const RHS &rhs)
		{
			return OP::apply(m_lhs, rhs);
		}
	};

	template<typename LHS>
	BaseValuePtr operator()(const LHS &lhs)
	{
		binary_op_rhs_dispatcher<LHS> rhs_visitor{lhs};
		return apply_visitor(rhs_visitor, m_rhs);
	}
};

struct is_true : public ValueVisitor<bool>
{
	template<typename T>
	bool operator()(const T &t)
	{
		return static_cast<bool>(t);
	}
};

// clang-format off
#define MAKE_BUILTIN_CALLABLE(function) { #function, sscript::make_builtin_function( #function , make_function(function)) }
// clang-format on

interpreter_visitor::interpreter_visitor()
    : m_globals(std::make_shared<SymbolTable>("Global")), m_symbolTable(m_globals)
{
	m_globals->Declare(MAKE_BUILTIN_CALLABLE(print));
	m_globals->Declare(MAKE_BUILTIN_CALLABLE(type));
	m_globals->Declare(MAKE_BUILTIN_CALLABLE(time));
	m_globals->Declare({"Integer", Integer::typeInfo});
	m_globals->Declare({"Boolean", Boolean::typeInfo});
	m_globals->Declare({"Float", Float::typeInfo});
	m_globals->Declare({"String", String::typeInfo});
}

interpreter_visitor::~interpreter_visitor() {}

void interpreter_visitor::Visit(ast::FloatPtr n) { PushValue(std::make_shared<Float>(n->GetNumber())); }

void interpreter_visitor::Visit(ast::IntegerPtr n) { PushValue(std::make_shared<Integer>(n->GetInteger())); }

void interpreter_visitor::Visit(ast::StringPtr s) { PushValue(std::make_shared<String>(s->GetString())); }

void interpreter_visitor::Visit(ast::IdentifierPtr i)
{
	PushValue(GetCurrentSymbolTable()->Get(i->GetIdentifier()).m_value);
}

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
			binary_op_dispatcher<bin_ops::add> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
		case ast::ArithmeticOp::types::Sub:
		{
			binary_op_dispatcher<bin_ops::sub> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
		case ast::ArithmeticOp::types::Mul:
		{
			binary_op_dispatcher<bin_ops::mul> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
		case ast::ArithmeticOp::types::Div:
		{
			binary_op_dispatcher<bin_ops::div> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
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
			unary_ops_dispatcher<unary_ops::negate> v;
			PushValue(apply_visitor(v, rhs));
			break;
		}
		case ast::Unary::types::Min:
		{
			unary_ops_dispatcher<unary_ops::minus> v;
			PushValue(apply_visitor(v, rhs));
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
			binary_op_dispatcher<bin_ops::eq> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
		case ast::ComparisonOp::types::Ne:
		{
			binary_op_dispatcher<bin_ops::ne> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
		case ast::ComparisonOp::types::Gt:
		{
			binary_op_dispatcher<bin_ops::gt> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
		case ast::ComparisonOp::types::Gte:
		{
			binary_op_dispatcher<bin_ops::gte> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
		case ast::ComparisonOp::types::Lt:
		{
			binary_op_dispatcher<bin_ops::lt> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
		case ast::ComparisonOp::types::Lte:
		{
			binary_op_dispatcher<bin_ops::lte> v(lhs, rhs);
			PushValue(apply_visitor(v, lhs));
			break;
		}
	}
}

void interpreter_visitor::Visit(ast::LogicOpPtr op)
{
	op->GetLhs()->AcceptVisitor(this);

	auto lhs = PopValue();

	is_true t;
	auto lhs_true = apply_visitor(t, lhs);
	switch (op->GetOperationType())
	{
		case ast::LogicOp::types::And:
		{
			if (lhs_true)
			{
				op->GetRhs()->AcceptVisitor(this);
				auto rhs = PopValue();
				PushValue(std::make_shared<Boolean>(apply_visitor(t, rhs)));
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
			PushValue(std::make_shared<Boolean>(apply_visitor(t, rhs)));
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

	is_true t;
	if (apply_visitor(t, condition))
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
	is_true t;
	while (true)
	{
		l->GetCondition()->AcceptVisitor(this);
		auto condition = PopValue();
		if (!apply_visitor(t, condition))
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
		GetCurrentSymbolTable()->Declare({v->GetIdentifier()->GetIdentifier(), PopValue()});
	}
	else
	{
		GetCurrentSymbolTable()->Declare({v->GetIdentifier()->GetIdentifier(), std::make_shared<NullObject>()});
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
	m_symbolTable = std::make_shared<SymbolTable>("Block", GetCurrentSymbolTable());
}

void interpreter_visitor::ExitBlock(const std::shared_ptr<SymbolTable> &previousSymbolTable)
{
	m_symbolTable = previousSymbolTable;
}

void interpreter_visitor::EnterFunction(const CallablePtr &callable)
{
	auto closure = callable->GetClosure();
	if (closure)
	{
		m_symbolTable = std::make_shared<SymbolTable>(callable->GetCallableName(), closure);
	}
	else
	{
		m_symbolTable = std::make_shared<SymbolTable>(callable->GetCallableName(), m_globals);
	}
}

void interpreter_visitor::ExitFunction(const std::shared_ptr<SymbolTable> &previousSymbolTable)
{
	m_symbolTable = previousSymbolTable;
}

void interpreter_visitor::Visit(ast::CallPtr c)
{
	c->GetCallee()->AcceptVisitor(this);
	auto popped = PopValue();
	auto callee = std::dynamic_pointer_cast<Callable>(popped);

	std::vector<BaseValuePtr> args;
	for (auto &a : c->GetArguments())
	{
		a->AcceptVisitor(this);
		args.push_back(PopValue());
	}

	if (!callee->IsVariadic() && callee->GetArity() != args.size())
	{
		throw std::runtime_error("Wrong arity");
	}

	auto prevSymbolTable = GetCurrentSymbolTable();
	EnterFunction(callee);
	try
	{
		callee->Call(this, args);
	}
	catch (BaseValuePtr &f)
	{
		ExitFunction(prevSymbolTable);
		PushValue(f);
		return;
	}

	ExitFunction(prevSymbolTable);
	PushValue(std::make_shared<NullObject>());
}

void interpreter_visitor::Visit(ast::FunctionDeclarationPtr func)
{
	auto identifier = func->GetIdentifier()->GetIdentifier();
	auto parameters = func->GetParameters();
	auto body = func->GetFunctionBody();

	auto callable = std::make_shared<Function>();
	callable->SetCallableBody(body);
	callable->SetCallableName(identifier);
	callable->SetClosure(GetCurrentSymbolTable());

	callable->SetParameters(parameters);
	callable->SetArity(parameters.size());

	GetCurrentSymbolTable()->Declare({identifier, callable});
}

void interpreter_visitor::Visit(ast::ParameterPtr par) { throw std::runtime_error("Unimplemented"); }

void interpreter_visitor::Visit(ast::ReturnPtr r)
{
	auto returnExression = r->GetReturnExpression();
	if (returnExression)
	{
		returnExression->AcceptVisitor(this);
		throw static_cast<BaseValuePtr>(PopValue());
	}

	throw static_cast<BaseValuePtr>(std::make_shared<NullObject>());
}

void interpreter_visitor::Visit(ast::ClassDeclarationPtr c)
{
	auto &identifier = c->GetIdentifier()->GetIdentifier();
	auto klass = std::make_shared<Class>(identifier);

	GetCurrentSymbolTable()->Declare({identifier, klass});

	for (auto &m : c->GetMethods())
	{
		auto callable = std::make_shared<Function>();
		callable->SetCallableBody(m->GetFunctionBody());
		callable->SetCallableName(m->GetIdentifier()->GetIdentifier());
		callable->SetClosure(GetCurrentSymbolTable());

		auto parameters = m->GetParameters();
		callable->SetParameters(parameters);
		callable->SetArity(parameters.size());

		klass->AddMethod(m->GetIdentifier()->GetIdentifier(), callable);
	}
}

void interpreter_visitor::Visit(ast::AnonymousMethodPtr a)
{
	a->GetInstance()->AcceptVisitor(this);
	auto instance = std::dynamic_pointer_cast<Instance>(PopValue());

	auto callable = std::make_shared<Function>();
	callable->SetCallableBody(a->GetBody());
	callable->SetCallableName("AnonymousMethod");
	callable->SetClosure(GetCurrentSymbolTable());
	callable->SetArity(0);

	callable = instance->Bind(callable, m_globals);
	auto prevSymbolTable = GetCurrentSymbolTable();

	EnterFunction(callable);
	std::vector<BaseValuePtr> args;
	try
	{
		callable->Call(this, args);
	}
	catch (BaseValuePtr &f)
	{
		ExitFunction(prevSymbolTable);
		PushValue(f);
		return;
	}
	ExitFunction(prevSymbolTable);

	// By default an anonymous method returns the instance
	PushValue(instance);
}

void interpreter_visitor::Visit(ast::GetExpressionPtr e)
{
	e->GetLhs()->AcceptVisitor(this);
	auto lhs = std::dynamic_pointer_cast<Instance>(PopValue());
	auto identifier = e->GetIdentifier()->GetIdentifier();

	auto value = lhs->GetMember(identifier);
	auto method = std::dynamic_pointer_cast<Function>(value);
	if (method)
	{
		PushValue(lhs->Bind(method, m_globals));
	}
	else
	{
		PushValue(value);
	}
}

void interpreter_visitor::Visit(ast::SetExpressionPtr e)
{
	e->GetLhs()->AcceptVisitor(this);
	auto lhs = std::dynamic_pointer_cast<Instance>(PopValue());
	auto identifier = e->GetIdentifier()->GetIdentifier();
	e->GetRhs()->AcceptVisitor(this);
	auto rhs = PopValue();

	lhs->GetLocalSymbolTable()->Declare({identifier, rhs});
	PushValue(rhs);
}

void interpreter_visitor::Visit(ast::ProgramPtr p)
{
	for (const auto &statement : p->GetStatements())
	{
		statement->AcceptVisitor(this);
	}
}

void interpreter_visitor::PushValue(const BaseValuePtr &v) { m_values.push(v); }

BaseValuePtr interpreter_visitor::PopValue()
{
	m_lastValue = m_values.top();
	m_values.pop();
	return m_lastValue;
}
}  // namespace sscript
