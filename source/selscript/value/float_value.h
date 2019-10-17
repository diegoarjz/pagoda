#pragma once

#include "base_value.h"
#include "type_info.h"
#include "undefined_operator.h"

namespace sscript
{
struct Integer;
struct Boolean;

struct Float : public BaseValue
{
	static const TypeInfoPtr typeInfo;

	Float();
	explicit Float(const float& f);
	virtual ~Float();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

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

	std::shared_ptr<Float> operator+(const Integer& i) const;
	std::shared_ptr<Float> operator-(const Integer& i) const;
	std::shared_ptr<Float> operator*(const Integer& i) const;
	std::shared_ptr<Float> operator/(const Integer& i) const;

	std::shared_ptr<Boolean> operator==(const Integer& f) const;
	std::shared_ptr<Boolean> operator!=(const Integer& f) const;
	std::shared_ptr<Boolean> operator<(const Integer& f) const;
	std::shared_ptr<Boolean> operator<=(const Integer& f) const;
	std::shared_ptr<Boolean> operator>(const Integer& f) const;
	std::shared_ptr<Boolean> operator>=(const Integer& f) const;

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

	std::shared_ptr<BaseValue> operator-() const { return std::make_shared<Float>(-m_value); }

	float m_value;
};
using FloatPtr = std::shared_ptr<Float>;
}  // namespace sscript
