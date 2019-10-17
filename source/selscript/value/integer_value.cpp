#include "integer_value.h"

#include "boolean_value.h"
#include "float_value.h"
#include "value_visitor.h"

#include <iostream>

namespace sscript
{
const TypeInfoPtr Integer::typeInfo = std::make_shared<TypeInfo>("Integer");

Integer::Integer() : BaseValue(typeInfo), m_value(0) {}
Integer::Integer(const int& f) : BaseValue(typeInfo), m_value(f) {}
Integer::~Integer() {}

std::string Integer::ToString() const { return std::to_string(m_value); }
void Integer::AcceptVisitor(ValueVisitorBase* v) { v->Visit(this); }

std::shared_ptr<Integer> Integer::operator+(const Integer& i) const
{
	return std::make_shared<Integer>(m_value + i.m_value);
}
std::shared_ptr<Integer> Integer::operator-(const Integer& i) const
{
	return std::make_shared<Integer>(m_value - i.m_value);
}
std::shared_ptr<Integer> Integer::operator*(const Integer& i) const
{
	return std::make_shared<Integer>(m_value * i.m_value);
}
std::shared_ptr<Integer> Integer::operator/(const Integer& i) const
{
	return std::make_shared<Integer>(m_value / i.m_value);
}

std::shared_ptr<Boolean> Integer::operator==(const Integer& i) const
{
	return std::make_shared<Boolean>(m_value == i.m_value);
}
std::shared_ptr<Boolean> Integer::operator!=(const Integer& i) const
{
	return std::make_shared<Boolean>(m_value != i.m_value);
}
std::shared_ptr<Boolean> Integer::operator<(const Integer& i) const
{
	return std::make_shared<Boolean>(m_value < i.m_value);
}
std::shared_ptr<Boolean> Integer::operator<=(const Integer& i) const
{
	return std::make_shared<Boolean>(m_value <= i.m_value);
}
std::shared_ptr<Boolean> Integer::operator>(const Integer& i) const
{
	return std::make_shared<Boolean>(m_value > i.m_value);
}
std::shared_ptr<Boolean> Integer::operator>=(const Integer& i) const
{
	return std::make_shared<Boolean>(m_value >= i.m_value);
}

std::shared_ptr<Float> Integer::operator+(const Float& f) const { return std::make_shared<Float>(m_value + f.m_value); }
std::shared_ptr<Float> Integer::operator-(const Float& f) const { return std::make_shared<Float>(m_value - f.m_value); }
std::shared_ptr<Float> Integer::operator*(const Float& f) const { return std::make_shared<Float>(m_value * f.m_value); }
std::shared_ptr<Float> Integer::operator/(const Float& f) const { return std::make_shared<Float>(m_value / f.m_value); }

std::shared_ptr<Boolean> Integer::operator==(const Float& f) const
{
	return std::make_shared<Boolean>(m_value == f.m_value);
}
std::shared_ptr<Boolean> Integer::operator!=(const Float& f) const
{
	return std::make_shared<Boolean>(m_value != f.m_value);
}
std::shared_ptr<Boolean> Integer::operator<(const Float& f) const
{
	return std::make_shared<Boolean>(m_value < f.m_value);
}
std::shared_ptr<Boolean> Integer::operator<=(const Float& f) const
{
	return std::make_shared<Boolean>(m_value <= f.m_value);
}
std::shared_ptr<Boolean> Integer::operator>(const Float& f) const
{
	return std::make_shared<Boolean>(m_value > f.m_value);
}
std::shared_ptr<Boolean> Integer::operator>=(const Float& f) const
{
	return std::make_shared<Boolean>(m_value >= f.m_value);
}
}  // namespace sscript
