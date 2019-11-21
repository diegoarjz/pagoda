#include "interpreter.h"

#include "dynamic_value/binding/make_free_function.h"
#include "dynamic_value/dynamic_value_table.h"
#include "dynamic_value/free_function_callable_body.h"
#include "dynamic_value/value_not_found.h"
#include "dynamic_value/vector3.h"
#include "dynamic_value/dynamic_plane.h"
#include "interpreter_visitor.h"

#include <stack>

namespace selector
{
template<class T>
std::shared_ptr<T> constructor_wrapper(const std::vector<DynamicValueBasePtr> &args)
{
	auto instance = T::DynamicConstructor(args);
	return instance;
}

class Interpreter::Impl
{
public:
	Impl()
	{
		RegisterBuiltinClass<Vector3>();
		RegisterBuiltinClass<DynamicPlane>();
	}

	~Impl() {}

	bool Interpret(const ast::ProgramPtr &program)
	{
		try
		{
			program->AcceptVisitor(&m_visitor);
		}
		catch (ValueNotFoundException &e)
		{
			m_visitor.GetCurrentSymbolTable()->DumpSymbols(std::cout);
			throw e;
		}

		return false;
	}

	void PushExternalSymbols(std::shared_ptr<DynamicValueTable> &externalSymbols)
	{
		if (m_externalSymbols.size() == 0)
		{
			m_visitor.GetGlobals()->SetParent(externalSymbols);
		}
		else
		{
			m_externalSymbols.top()->SetParent(externalSymbols);
		}
		m_externalSymbols.push(externalSymbols);
	}

	void PopExternalSymbols()
	{
		if (m_externalSymbols.size() == 0)
		{
			return;
		}

		auto topExternalSymbols = m_externalSymbols.top();
		m_externalSymbols.pop();

		if (m_externalSymbols.size() == 0)
		{
			m_visitor.GetGlobals()->SetParent(nullptr);
		}
		else
		{
			m_externalSymbols.top()->SetParent(nullptr);
		}
	}

	DynamicValueBasePtr GetLastEvaluatedExpression() const { return m_visitor.GetLastEvaluatedExpression(); }

	template<class T>
	void RegisterBuiltinClass()
	{
		using ConstructorFunc_t = std::function<std::shared_ptr<T>(const std::vector<DynamicValueBasePtr> &)>;
		using FreeFunctionCallableBody_t = FreeFunctionCallableBody<ConstructorFunc_t>;
		std::shared_ptr<FreeFunctionCallableBody_t> constructor =
		    std::make_shared<FreeFunctionCallableBody_t>(constructor_wrapper<T>);
		std::shared_ptr<Function> constructorFunction = std::make_shared<Function>(constructor);

		constructorFunction->SetVariadic(true);

		m_visitor.GetCurrentSymbolTable()->Declare(T::s_typeInfo->GetTypeName(), constructorFunction);
	}

private:
	std::stack<std::shared_ptr<DynamicValueTable>> m_externalSymbols;
	interpreter_visitor m_visitor;
};

Interpreter::Interpreter() : m_implementation(std::make_unique<typename Interpreter::Impl>()) {}

Interpreter::~Interpreter() {}

bool Interpreter::Interpret(const ast::ProgramPtr &program) { return m_implementation->Interpret(program); }

void Interpreter::PushExternalSymbols(std::shared_ptr<DynamicValueTable> &externalSymbols)
{
	m_implementation->PushExternalSymbols(externalSymbols);
}

void Interpreter::PopExternalSymbols() { m_implementation->PopExternalSymbols(); }

DynamicValueBasePtr Interpreter::GetLastEvaluatedExpression() const
{
	return m_implementation->GetLastEvaluatedExpression();
}
}  // namespace selector
