#include "integer_value.h"

#include "float_value.h"
#include "string_value.h"
#include "type_info.h"
#include "value_visitor.h"

namespace pagoda::dynamic
{
const TypeInfoPtr Integer::s_typeInfo = std::make_shared<TypeInfo>("Integer");

Integer::Integer() : DynamicValueBase(s_typeInfo), m_value(0) {}
Integer::Integer(const int& i) : DynamicValueBase(s_typeInfo), m_value(i) {}
Integer::Integer(const float& f) : DynamicValueBase(s_typeInfo), m_value(f) {}
Integer::Integer(const FloatValuePtr& f) : DynamicValueBase(s_typeInfo), m_value(static_cast<float>(*f)) {}
Integer::Integer(const FloatValue& f) : DynamicValueBase(s_typeInfo), m_value(static_cast<float>(f)) {}
Integer::~Integer() {}

Integer::operator int() const { return m_value; }
Integer::operator float() const { return m_value; }

Integer& Integer::operator=(const int& i)
{
	m_value = i;
	return *this;
}

Integer& Integer::operator=(const float& f)
{
	m_value = f;
	return *this;
}

std::string Integer::ToString() const { return std::to_string(m_value); }

Integer Integer::operator-() const { return Integer(-m_value); }

Integer Integer::operator+(const Integer& i) const { return Integer(m_value + i.m_value); }
Integer Integer::operator-(const Integer& i) const { return Integer(m_value - i.m_value); }
Integer Integer::operator*(const Integer& i) const { return Integer(m_value * i.m_value); }
Integer Integer::operator/(const Integer& i) const { return Integer(m_value / i.m_value); }

FloatValue Integer::operator+(const FloatValue& f) const { return FloatValue(m_value + static_cast<float>(f)); }
FloatValue Integer::operator-(const FloatValue& f) const { return FloatValue(m_value - static_cast<float>(f)); }
FloatValue Integer::operator*(const FloatValue& f) const { return FloatValue(m_value * static_cast<float>(f)); }
FloatValue Integer::operator/(const FloatValue& f) const { return FloatValue(m_value / static_cast<float>(f)); }

String Integer::operator*(const String& s) const { return s * (*this); }

bool Integer::operator==(const Integer& i) const { return m_value == i.m_value; }
bool Integer::operator!=(const Integer& i) const { return m_value != i.m_value; }
bool Integer::operator<(const Integer& i) const { return m_value < i.m_value; }
bool Integer::operator<=(const Integer& i) const { return m_value <= i.m_value; }
bool Integer::operator>(const Integer& i) const { return m_value > i.m_value; }
bool Integer::operator>=(const Integer& i) const { return m_value >= i.m_value; }

bool Integer::operator==(const FloatValue& f) const { return m_value == static_cast<float>(f); }
bool Integer::operator!=(const FloatValue& f) const { return m_value != static_cast<float>(f); }
bool Integer::operator<(const FloatValue& f) const { return m_value < static_cast<float>(f); }
bool Integer::operator<=(const FloatValue& f) const { return m_value <= static_cast<float>(f); }
bool Integer::operator>(const FloatValue& f) const { return m_value > static_cast<float>(f); }
bool Integer::operator>=(const FloatValue& f) const { return m_value >= static_cast<float>(f); }

void Integer::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }
}  // namespace pagoda::dynamic
