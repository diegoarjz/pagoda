#pragma once

#include "base_value.h"
#include "class_value.h"
#include "type_info.h"
#include "undefined_operator.h"

namespace sscript
{
class SymbolTable;

class Instance : public BaseValue
{
public:
	static const TypeInfoPtr typeInfo;

	Instance();
	Instance(const ClassPtr& klass);
	virtual ~Instance();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	ClassPtr GetClass() const { return m_class; }
	BaseValuePtr GetMember(const std::string& name);
	std::shared_ptr<SymbolTable>& GetLocalSymbolTable() { return m_symbolTable; }
	virtual FunctionPtr Bind(FunctionPtr& function, const std::shared_ptr<SymbolTable>& globals);

	void SetClassType(const ClassPtr& klass);

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

	bool operator!() const { throw UndefinedUnaryOperatorException("!", typeInfo); }
	explicit operator bool() const { throw UndefinedUnaryOperatorException("conversion to boolean", typeInfo); }

	std::shared_ptr<BaseValue> operator-() const { throw UndefinedUnaryOperatorException("-", typeInfo); }

private:
	ClassPtr m_class;
	std::shared_ptr<SymbolTable> m_symbolTable;
};
}  // namespace sscript
