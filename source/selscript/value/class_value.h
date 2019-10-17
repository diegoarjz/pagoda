#pragma once

#include "base_value.h"
#include "callable.h"
#include "type_info.h"
#include "undefined_operator.h"

namespace sscript
{
struct Function;
using FunctionPtr = std::shared_ptr<Function>;
class SymbolTable;

class Class : public BaseValue, public Callable
{
public:
	static const TypeInfoPtr typeInfo;

	Class(const std::string& className);
	virtual ~Class();

	std::string ToString() const override;
	void AcceptVisitor(ValueVisitorBase* v) override;

	void AddMethod(const std::string& methodName, FunctionPtr& callable);
	std::shared_ptr<SymbolTable>& GetSymbolTable() { return m_symbolTable; }

	void SetCallableName(const std::string&) override;
	const std::string& GetCallableName() const override;
	void SetArity(const std::size_t&) override;
	const std::size_t& GetArity() const override;
	bool IsVariadic() const override;
	void SetVariadic(bool) override;
	void SetParameters(const std::vector<ast::ParameterPtr>&) override;
	const std::vector<ast::ParameterPtr>& GetParameters() const override;
	void SetClosure(const std::shared_ptr<SymbolTable>&) override;
	const std::shared_ptr<SymbolTable>& GetClosure() const override;
	void SetCallableBody(ast::StatementBlockPtr) override;

	void Call(interpreter_visitor*, const std::vector<BaseValuePtr>&) override;

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
	FunctionPtr getConstructor() const;

	std::string m_className;
	mutable FunctionPtr m_constructor;
	std::shared_ptr<SymbolTable> m_symbolTable;
};
using ClassPtr = std::shared_ptr<Class>;

}  // namespace sscript
