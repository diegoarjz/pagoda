#pragma once

#include "base_value.h"
#include "type_info.h"
#include "undefined_operator.h"

namespace sscript
{
struct Boolean : public BaseValue
{
	static const TypeInfoPtr typeInfo;

	Boolean();
	explicit Boolean(const bool& b);
	virtual ~Boolean();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	std::shared_ptr<Boolean> operator==(const Boolean& b) const
	{
		return std::make_shared<Boolean>(m_value == b.m_value);
	}
	std::shared_ptr<Boolean> operator!=(const Boolean& b) const
	{
		return std::make_shared<Boolean>(m_value != b.m_value);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator+(const T& o) const
	{
		throw UndefinedBinaryOperatorException("+", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator-(const T& o) const
	{
		throw UndefinedBinaryOperatorException("-", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator*(const T& o) const
	{
		throw UndefinedBinaryOperatorException("*", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator/(const T& o) const
	{
		throw UndefinedBinaryOperatorException("/", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator==(const T& o) const
	{
		throw UndefinedBinaryOperatorException("==", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator!=(const T& o) const
	{
		throw UndefinedBinaryOperatorException("!=", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator<(const T& o) const
	{
		throw UndefinedBinaryOperatorException("<", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator<=(const T& o) const
	{
		throw UndefinedBinaryOperatorException("<=", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator>(const T& o) const
	{
		throw UndefinedBinaryOperatorException(">", typeInfo, T::typeInfo);
	}

	template<class T>
	std::shared_ptr<BaseValue> operator>=(const T& o) const
	{
		throw UndefinedBinaryOperatorException(">=", typeInfo, T::typeInfo);
	}

	bool operator!() const { return !m_value; }
	explicit operator bool() const { return m_value; }

	std::shared_ptr<BaseValue> operator-() const { throw UndefinedUnaryOperatorException("-", typeInfo); }

	bool m_value;
};
using BooleanPtr = std::shared_ptr<Boolean>;
}  // namespace sscript
