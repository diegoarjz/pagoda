#include "dynamic_value_base.h"

#include "type_info.h"
#include "value_visitor.h"

namespace pagoda
{
const TypeInfoPtr DynamicValueBase::s_typeInfo = std::make_shared<TypeInfo>("DynamicValueBase");

DynamicValueBase::DynamicValueBase(TypeInfoPtr typeInfo) : m_typeInfo(typeInfo) {}
DynamicValueBase::~DynamicValueBase() {}
TypeInfoPtr DynamicValueBase::GetTypeInfo() const { return m_typeInfo; }
}  // namespace pagoda
