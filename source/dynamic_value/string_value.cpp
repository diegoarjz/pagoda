#include "string_value.h"

#include "integer_value.h"
#include "type_info.h"
#include "value_visitor.h"

namespace selector
{
const TypeInfoPtr String::s_typeInfo = std::make_shared<TypeInfo>("String");

String::String() : DynamicValueBase(s_typeInfo) {}
String::String(const std::string& s) : DynamicValueBase(s_typeInfo), m_value(s) {}
String::~String() {}

std::string String::ToString() const { return m_value; }

String::operator std::string() const { return m_value; }

String& String::operator=(const std::string& s)
{
	m_value = s;
	return *this;
}

String String::operator+(const String& s) const { return String(m_value + s.m_value); }
String String::operator*(const Integer& i) const
{
	std::string s;
	for (auto c = 0u; c < static_cast<int>(i); ++c)
	{
		s += m_value;
	}
	return String(s);
}
String String::operator+(const Integer& i) const { return String(m_value + std::to_string(static_cast<int>(i))); }

bool String::operator==(const String& s) const { return m_value == s.m_value; }
bool String::operator!=(const String& s) const { return m_value != s.m_value; }

void String::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }
}  // namespace selector
