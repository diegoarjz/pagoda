#pragma once

#include "base_value.h"
#include "type_info.h"
#include "undefined_operator.h"

namespace sscript
{
struct NullObject : public BaseValue
{
	static const TypeInfoPtr typeInfo;

	NullObject();
	virtual ~NullObject();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

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

	bool operator!() const { return true; }
	explicit operator bool() const { return false; }

	std::shared_ptr<BaseValue> operator-() const { throw UndefinedUnaryOperatorException("-", typeInfo); }
};
using NullObjectPtr = std::shared_ptr<NullObject>;

}  // namespace sscript
