#pragma once

#include <stdexcept>
#include <string>
#include <memory>

namespace sscript
{
class TypeInfo;
using TypeInfoPtr = std::shared_ptr<TypeInfo>;

class UndefinedBinaryOperatorException : public std::runtime_error
{
public:
	UndefinedBinaryOperatorException(const std::string &op, const TypeInfoPtr &lhsType, const TypeInfoPtr &rhsType);
	virtual ~UndefinedBinaryOperatorException();

	const TypeInfoPtr &GetLhsType() const { return m_lhsType; }
	const TypeInfoPtr &GetRhsType() const { return m_rhsType; }

private:
	std::string m_operatorName;
	const TypeInfoPtr &m_lhsType;
	const TypeInfoPtr &m_rhsType;
};

class UndefinedUnaryOperatorException : public std::runtime_error
{
public:
	UndefinedUnaryOperatorException(const std::string &op, const TypeInfoPtr &operand);
	virtual ~UndefinedUnaryOperatorException();

	const TypeInfoPtr &GetOperandType() const { return m_operandType; }

private:
	std::string m_operatorName;
	const TypeInfoPtr &m_operandType;
};
}  // namespace sscript
