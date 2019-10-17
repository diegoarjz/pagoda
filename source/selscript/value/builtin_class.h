#pragma once

#include "base_value.h"
#include "class_value.h"
#include "type_info.h"
#include "undefined_operator.h"
#include "value_visitor.h"

#include "builtin_function.h"
#include "builtin_instance.h"
#include "function.h"

#include "../interpreter/symbol_table.h"

#include "../intermediate/ast.h"

#include <iostream>

namespace sscript
{
struct ValueVisitorBase;
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

template<class I>
class BuiltinClass : public Class
{
public:
	const TypeInfoPtr typeInfo = std::make_shared<TypeInfo>("BuiltinClass");

	BuiltinClass(const std::string& className) : Class(className)
	{
		auto constructor = std::make_shared<Function>();
		constructor->SetCallableBody(std::make_shared<ast::StatementBlock>());
	}

	~BuiltinClass() {}

	std::string ToString() const override { return "<BuiltinClass>"; }
	void AcceptVisitor(ValueVisitorBase* v) override { v->Visit(this); }

	void SetConstructor(const FunctionPtr& constructor) { m_constructor = constructor; }
	void Call(interpreter_visitor* v, const std::vector<BaseValuePtr>& args) override { m_constructor->Call(v, args); }

	virtual const std::size_t& GetArity() const override
	{
		static const std::size_t arity = 0;
		return arity;
	}
	virtual bool IsVariadic() const override { return false; }
	virtual const std::vector<ast::ParameterPtr>& GetParameters() const override
	{
		static const std::vector<std::string> parameters;
		return parameters;
	}
	virtual const std::shared_ptr<SymbolTable>& GetClosure() const override
	{
		static const std::shared_ptr<SymbolTable> n;
		return n;
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

	bool operator!() const { throw UndefinedUnaryOperatorException("!", typeInfo); }
	explicit operator bool() const { throw UndefinedUnaryOperatorException("conversion to boolean", typeInfo); }

	std::shared_ptr<BaseValue> operator-() const { throw UndefinedUnaryOperatorException("-", typeInfo); }

private:
	FunctionPtr m_constructor;
};

}  // namespace sscript
