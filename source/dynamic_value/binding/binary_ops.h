#pragma once

#include "../dynamic_value_base.h"
#include "../value_visitor.h"
#include "has_operators.h"

namespace selector
{
template<class LHS, class RHS>
class UndefinedBinaryOperator : public std::runtime_error
{
public:
	UndefinedBinaryOperator(const std::string &op)
	    : std::runtime_error("Undefined " + op + " operator between " + LHS::s_typeInfo->GetTypeName() + " and " +
	                         RHS::s_typeInfo->GetTypeName())
	{
	}
};

struct add
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_add_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		return std::make_shared<decltype(lhs + rhs)>(lhs + rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_add_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                    const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>("+");
	}
};

struct sub
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_sub_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		return std::make_shared<decltype(lhs - rhs)>(lhs - rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_sub_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                    const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>("-");
	}
};

struct mul
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_mul_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		return std::make_shared<decltype(lhs * rhs)>(lhs * rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_mul_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                    const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>("*");
	}
};

struct div
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_div_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		return std::make_shared<decltype(lhs / rhs)>(lhs / rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_div_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                    const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>("/");
	}
};

struct eq
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_eq_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                  const RHS &rhs)
	{
		return std::make_shared<Boolean>(lhs == rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_eq_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>("==");
	}
};

struct neq
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_neq_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		return std::make_shared<Boolean>(lhs != rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_neq_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                    const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>("!=");
	}
};

struct gt
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_gt_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                  const RHS &rhs)
	{
		return std::make_shared<Boolean>(lhs > rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_gt_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>(">");
	}
};

struct gte
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_gte_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		return std::make_shared<Boolean>(lhs >= rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_gte_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                    const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>(">=");
	}
};

struct lt
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_lt_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                  const RHS &rhs)
	{
		return std::make_shared<Boolean>(lhs < rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_lt_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>("<");
	}
};

struct lte
{
	template<typename LHS, typename RHS>
	static typename std::enable_if<has_lte_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                   const RHS &rhs)
	{
		return std::make_shared<Boolean>(lhs <= rhs);
	}

	template<typename LHS, typename RHS>
	static typename std::enable_if<!has_lte_operator<LHS, RHS>::value, DynamicValueBasePtr>::type apply(const LHS &lhs,
	                                                                                                    const RHS &rhs)
	{
		throw UndefinedBinaryOperator<LHS, RHS>("<=");
	}
};

template<class OP>
struct binary_op_dispatcher : public ValueVisitor<DynamicValueBasePtr>
{
	DynamicValueBasePtr m_lhs;
	DynamicValueBasePtr m_rhs;

	binary_op_dispatcher(const DynamicValueBasePtr &lhs, const DynamicValueBasePtr &rhs) : m_lhs(lhs), m_rhs(rhs) {}

	template<typename LHS>
	struct binary_op_rhs_dispatcher : public ValueVisitor<DynamicValueBasePtr>
	{
		const LHS &m_lhs;
		explicit binary_op_rhs_dispatcher(const LHS &lhs) : m_lhs(lhs) {}

		template<typename RHS>
		DynamicValueBasePtr operator()(const RHS &rhs)
		{
			return OP::apply(m_lhs, rhs);
		}
	};

	template<typename LHS>
	DynamicValueBasePtr operator()(const LHS &lhs)
	{
		binary_op_rhs_dispatcher<LHS> rhs_visitor{lhs};
		return apply_visitor(rhs_visitor, *m_rhs);
	}
};
}  // namespace selector
