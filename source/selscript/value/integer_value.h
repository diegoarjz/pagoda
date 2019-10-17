#pragma once

#include "base_value.h"
#include "type_info.h"
#include "undefined_operator.h"

namespace sscript
{
struct Float;
struct Boolean;

struct Integer : public BaseValue
{
	static const TypeInfoPtr typeInfo;

	Integer();
	explicit Integer(const int& f);
	virtual ~Integer();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	std::shared_ptr<Integer> operator+(const Integer& i) const;
	std::shared_ptr<Integer> operator-(const Integer& i) const;
	std::shared_ptr<Integer> operator*(const Integer& i) const;
	std::shared_ptr<Integer> operator/(const Integer& i) const;

	std::shared_ptr<Boolean> operator==(const Integer& i) const;
	std::shared_ptr<Boolean> operator!=(const Integer& i) const;
	std::shared_ptr<Boolean> operator<(const Integer& i) const;
	std::shared_ptr<Boolean> operator<=(const Integer& i) const;
	std::shared_ptr<Boolean> operator>(const Integer& i) const;
	std::shared_ptr<Boolean> operator>=(const Integer& i) const;

	std::shared_ptr<Float> operator+(const Float& f) const;
	std::shared_ptr<Float> operator-(const Float& f) const;
	std::shared_ptr<Float> operator*(const Float& f) const;
	std::shared_ptr<Float> operator/(const Float& f) const;

	std::shared_ptr<Boolean> operator==(const Float& f) const;
	std::shared_ptr<Boolean> operator!=(const Float& f) const;
	std::shared_ptr<Boolean> operator<(const Float& f) const;
	std::shared_ptr<Boolean> operator<=(const Float& f) const;
	std::shared_ptr<Boolean> operator>(const Float& f) const;
	std::shared_ptr<Boolean> operator>=(const Float& f) const;

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

	bool operator!() const { return m_value == 0; }
	explicit operator bool() const { return m_value != 0; }

	std::shared_ptr<BaseValue> operator-() const { return std::make_shared<Integer>(-m_value); }

	int m_value;
};
using IntegerPtr = std::shared_ptr<Integer>;
}  // namespace sscript
