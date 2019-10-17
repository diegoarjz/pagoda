#include "string_value.h"

#include "boolean_value.h"
#include "integer_value.h"
#include "value_visitor.h"

namespace sscript
{
const TypeInfoPtr String::typeInfo = std::make_shared<TypeInfo>("String");

String::String() : BaseValue(typeInfo) {}
String::String(const std::string& s) : BaseValue(typeInfo), m_value(s) {}
String::~String() {}

std::string String::ToString() const { return m_value; }
void String::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }

std::shared_ptr<String> String::operator*(const Integer& rhs) const
{
	std::string r;
	for (auto i = 0u; i < static_cast<uint32_t>(rhs.m_value); ++i)
	{
		r += m_value;
	}
	return std::make_shared<String>(r);
}

std::shared_ptr<String> String::operator+(const String& rhs) const
{
	return std::make_shared<String>(m_value + rhs.m_value);
}

std::shared_ptr<Boolean> String::operator==(const String& rhs) const
{
	return std::make_shared<Boolean>(m_value == rhs.m_value);
}
std::shared_ptr<Boolean> String::operator!=(const String& rhs) const
{
	return std::make_shared<Boolean>(m_value != rhs.m_value);
}
}  // namespace sscript
