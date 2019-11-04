#include "float_value.h"

#include "integer_value.h"
#include "type_info.h"
#include "value_visitor.h"

namespace selector
{
const TypeInfoPtr FloatValue::s_typeInfo = std::make_shared<TypeInfo>("Float");

FloatValue::FloatValue() : DynamicValueBase(s_typeInfo), m_value(0) {}
FloatValue::FloatValue(const float& f) : DynamicValueBase(s_typeInfo), m_value(f) {}
FloatValue::FloatValue(const int& i) : DynamicValueBase(s_typeInfo), m_value(i) {}
FloatValue::FloatValue(const IntegerPtr& i) : DynamicValueBase(s_typeInfo), m_value(static_cast<int>(*i)) {}
FloatValue::FloatValue(const Integer& i) : DynamicValueBase(s_typeInfo), m_value(static_cast<int>(i)) {}
FloatValue::~FloatValue() {}

FloatValue::operator float() const { return m_value; }
FloatValue::operator int() const { return m_value; }
FloatValue& FloatValue::operator=(const float& f)
{
	m_value = f;
	return *this;
}

FloatValue& FloatValue::operator=(const int& f)
{
	m_value = f;
	return *this;
}

std::string FloatValue::ToString() const { return std::to_string(m_value); }

FloatValue FloatValue::operator+(const FloatValue& f) const { return FloatValue(m_value + f.m_value); }
FloatValue FloatValue::operator-(const FloatValue& f) const { return FloatValue(m_value - f.m_value); }
FloatValue FloatValue::operator*(const FloatValue& f) const { return FloatValue(m_value * f.m_value); }
FloatValue FloatValue::operator/(const FloatValue& f) const { return FloatValue(m_value / f.m_value); }
FloatValue FloatValue::operator-() const { return FloatValue(-m_value); }

FloatValue FloatValue::operator+(const Integer& i) const { return FloatValue(m_value + static_cast<int>(i)); }
FloatValue FloatValue::operator-(const Integer& i) const { return FloatValue(m_value - static_cast<int>(i)); }
FloatValue FloatValue::operator*(const Integer& i) const { return FloatValue(m_value * static_cast<int>(i)); }
FloatValue FloatValue::operator/(const Integer& i) const { return FloatValue(m_value / static_cast<int>(i)); }

bool FloatValue::operator==(const FloatValue& f) const { return m_value == f.m_value; }
bool FloatValue::operator!=(const FloatValue& f) const { return m_value != f.m_value; }
bool FloatValue::operator<(const FloatValue& f) const { return m_value < f.m_value; }
bool FloatValue::operator<=(const FloatValue& f) const { return m_value <= f.m_value; }
bool FloatValue::operator>(const FloatValue& f) const { return m_value > f.m_value; }
bool FloatValue::operator>=(const FloatValue& f) const { return m_value >= f.m_value; }

bool FloatValue::operator==(const Integer& f) const { return m_value == static_cast<int>(f); }
bool FloatValue::operator!=(const Integer& f) const { return m_value != static_cast<int>(f); }
bool FloatValue::operator<(const Integer& f) const { return m_value < static_cast<int>(f); }
bool FloatValue::operator<=(const Integer& f) const { return m_value <= static_cast<int>(f); }
bool FloatValue::operator>(const Integer& f) const { return m_value > static_cast<int>(f); }
bool FloatValue::operator>=(const Integer& f) const { return m_value >= static_cast<int>(f); }

void FloatValue::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }
}  // namespace selector
