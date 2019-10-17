#include "interpreter.h"

#include "interpreter_visitor.h"
#include "symbol_table.h"

#include <stack>

namespace sscript
{
class Interpreter::Impl
{
public:
	Impl() {}
	~Impl() {}

	bool Interpret(const ast::ProgramPtr &program)
	{
		try
		{
			program->AcceptVisitor(&m_visitor);
		}
		catch (SymbolNotFoundException &e)
		{
			m_visitor.GetCurrentSymbolTable()->DumpSymbols();
			throw e;
		}

		return false;
	}

	void PushExternalSymbols(std::shared_ptr<SymbolTable> &externalSymbols)
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

	BaseValuePtr GetLastEvaluatedExpression() const { return m_visitor.GetLastEvaluatedExpression(); }

private:
	std::stack<std::shared_ptr<SymbolTable>> m_externalSymbols;
	interpreter_visitor m_visitor;
};

Interpreter::Interpreter() : m_implementation(std::make_unique<typename Interpreter::Impl>()) {}

Interpreter::~Interpreter() {}

bool Interpreter::Interpret(const ast::ProgramPtr &program) { return m_implementation->Interpret(program); }

void Interpreter::PushExternalSymbols(std::shared_ptr<SymbolTable> &externalSymbols)
{
	m_implementation->PushExternalSymbols(externalSymbols);
}

void Interpreter::PopExternalSymbols() { m_implementation->PopExternalSymbols(); }

BaseValuePtr Interpreter::GetLastEvaluatedExpression() const { return m_implementation->GetLastEvaluatedExpression(); }
}  // namespace sscript
