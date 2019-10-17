#include "class_value.h"

#include "function.h"
#include "instance_value.h"
#include "value_visitor.h"

#include "../interpreter/interpreter_visitor.h"
#include "../interpreter/symbol_table.h"

namespace sscript
{
const TypeInfoPtr Class::typeInfo = std::make_shared<TypeInfo>("Class");

Class::Class(const std::string& className)
    : BaseValue(std::make_shared<TypeInfo>(className)),
      m_className(className),
      m_symbolTable(std::make_shared<SymbolTable>(className))
{
}

Class::~Class() {}

std::string Class::ToString() const { return "<Class:" + m_className + ">"; }

void Class::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }

void Class::AddMethod(const std::string& methodName, FunctionPtr& callable)
{
	m_symbolTable->Declare({methodName, callable});
}

void Class::SetCallableName(const std::string&) {}

const std::string& Class::GetCallableName() const { return m_className; }

void Class::SetArity(const std::size_t&) {}

const std::size_t& Class::GetArity() const { return getConstructor()->GetArity(); }

bool Class::IsVariadic() const { return getConstructor()->IsVariadic(); }

void Class::SetVariadic(bool) {}

void Class::SetParameters(const std::vector<ast::ParameterPtr>&) {}

const std::vector<ast::ParameterPtr>& Class::GetParameters() const { return getConstructor()->GetParameters(); }

void Class::SetClosure(const std::shared_ptr<SymbolTable>&) {}

const std::shared_ptr<SymbolTable>& Class::GetClosure() const { return getConstructor()->GetClosure(); }

void Class::SetCallableBody(ast::StatementBlockPtr) {}

void Class::Call(interpreter_visitor* v, const std::vector<BaseValuePtr>& args)
{
	auto prevSymbolTable = v->GetCurrentSymbolTable();
	auto instance = std::make_shared<Instance>(std::dynamic_pointer_cast<Class>(shared_from_this()));
	instance->SetClassType(std::dynamic_pointer_cast<Class>(shared_from_this()));
	auto constructor = getConstructor();
	constructor = instance->Bind(constructor, v->GetGlobals());
	v->EnterFunction(constructor);
	constructor->Call(v, args);
	v->ExitFunction(prevSymbolTable);
	throw static_cast<BaseValuePtr>(instance);
}

FunctionPtr Class::getConstructor() const
{
	if (m_constructor)
	{
		return m_constructor;
	}

	auto symbol = m_symbolTable->Get(m_className);
	m_constructor = std::dynamic_pointer_cast<Function>(symbol.m_value);

	return m_constructor;
}
}  // namespace sscript
