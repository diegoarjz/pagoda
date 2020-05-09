#include "boolean_value.h"

#include "type_info.h"
#include "value_visitor.h"

namespace pagoda
{
const TypeInfoPtr Boolean::s_typeInfo = std::make_shared<TypeInfo>("Boolean");

Boolean::Boolean() : DynamicValueBase(s_typeInfo), m_value(false) {}
Boolean::Boolean(const bool& b) : DynamicValueBase(s_typeInfo), m_value(b) {}
Boolean::~Boolean() {}

std::string Boolean::ToString() const { return (m_value ? "true" : "false"); }

Boolean::operator bool() const { return m_value; }

Boolean& Boolean::operator=(const bool& b)
{
	m_value = b;
	return *this;
}

Boolean Boolean::operator==(const Boolean& b) const { return Boolean(m_value == b.m_value); }
Boolean Boolean::operator!=(const Boolean& b) const { return Boolean(m_value != b.m_value); }

Boolean Boolean::operator!() const { return Boolean(!m_value); }

void Boolean::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }
}  // namespace pagoda
