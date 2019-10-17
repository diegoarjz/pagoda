#include "undefined_operator.h"

#include "type_info.h"

namespace sscript
{
UndefinedBinaryOperatorException::UndefinedBinaryOperatorException(const std::string &op, const TypeInfoPtr &lhsType,
                                                                   const TypeInfoPtr &rhsType)
    : std::runtime_error("Undefined Binary Operator " + op + " between " + lhsType->Name() + " and " + rhsType->Name()),
      m_operatorName(op),
      m_lhsType(lhsType),
      m_rhsType(rhsType)
{
}

UndefinedBinaryOperatorException::~UndefinedBinaryOperatorException() {}

UndefinedUnaryOperatorException::UndefinedUnaryOperatorException(const std::string &op, const TypeInfoPtr &operand)
    : std::runtime_error("Undefined Unary Operator " + op + " for " + operand->Name()),
      m_operatorName(op),
      m_operandType(operand)
{
}

UndefinedUnaryOperatorException::~UndefinedUnaryOperatorException() {}
}  // namespace sscript
