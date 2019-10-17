#include "instance_value.h"

#include "function.h"
#include "value_visitor.h"

#include "../interpreter/symbol_table.h"

namespace sscript
{
const TypeInfoPtr Instance::typeInfo = std::make_shared<TypeInfo>("Instance");

Instance::Instance() : BaseValue(nullptr), m_class(nullptr), m_symbolTable(nullptr) {}

Instance::Instance(const ClassPtr& klass)
    : BaseValue(klass->m_type),
      m_class(klass),
      m_symbolTable(std::make_shared<SymbolTable>(klass->m_type->Name(), klass->GetSymbolTable()))
{
}

Instance::~Instance() {}

std::string Instance::ToString() const { return "<Instance:" + m_type->Name() + ">"; }

void Instance::SetClassType(const ClassPtr& klass)
{
	m_class = klass;
	m_type = m_class->m_type;
	m_symbolTable = std::make_shared<SymbolTable>(klass->m_type->Name(), klass->GetSymbolTable());
}

void Instance::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }

BaseValuePtr Instance::GetMember(const std::string& name) { return m_symbolTable->Get(name).m_value; }

FunctionPtr Instance::Bind(FunctionPtr& function, const std::shared_ptr<SymbolTable>& globals)
{
	auto boundMethod = std::make_shared<Function>(*function);
	auto closure = std::make_shared<SymbolTable>(m_type->Name(), globals);
	closure->Declare({"this", shared_from_this()});
	boundMethod->SetClosure(closure);
	boundMethod->SetBoundInstance(std::dynamic_pointer_cast<Instance>(shared_from_this()));
	return boundMethod;
}

}  // namespace sscript
