#pragma once

#include "base_value.h"
#include "type_info.h"
#include "undefined_operator.h"

namespace sscript
{
struct Integer;
struct Boolean;

struct String : public BaseValue
{
	static const TypeInfoPtr typeInfo;

	String();
	explicit String(const std::string& s);
	virtual ~String();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	std::shared_ptr<String> operator*(const Integer& rhs) const;
	std::shared_ptr<String> operator+(const String& rhs) const;

	std::shared_ptr<Boolean> operator==(const String& rhs) const;
	std::shared_ptr<Boolean> operator!=(const String& rhs) const;

	template<class T>
	std::shared_ptr<BaseValue> operator+(const T& o) const
	{
		return std::make_shared<String>(m_value + o.ToString());
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

	bool operator!() const { return m_value.size() == 0; }
	explicit operator bool() const { return m_value.size() != 0; }

	std::shared_ptr<BaseValue> operator-() const { throw UndefinedUnaryOperatorException("-", typeInfo); }

	std::string m_value;
};
using StringPtr = std::shared_ptr<String>;
}  // namespace sscript
