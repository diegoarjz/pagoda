#pragma once

namespace selector
{
/**
 * Tests whether the \c DynamicValueBase of type T can be added by V.
 */
template<typename T, typename V>
class has_add_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C, typename R>
	static yes test(decltype(static_cast<R (C::*)(const V&) const>(&C::operator+)));

	template<typename C, typename R>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T, T>(0)) == sizeof(yes) || sizeof(test<T, V>(0)) == sizeof(yes))
	};
};

template<typename T, typename V>
class has_sub_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C, typename R>
	static yes test(decltype(static_cast<R (C::*)(const V&) const>(&C::operator-)));

	template<typename C, typename R>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T, T>(0)) == sizeof(yes) || sizeof(test<T, V>(0)) == sizeof(yes))
	};
};

template<typename T, typename V>
class has_mul_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C, typename R>
	static yes test(decltype(static_cast<R (C::*)(const V&) const>(&C::operator*)));

	template<typename C, typename R>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T, T>(0)) == sizeof(yes) || sizeof(test<T, V>(0)) == sizeof(yes))
	};
};

template<typename T, typename V>
class has_div_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C, typename R>
	static yes test(decltype(static_cast<R (C::*)(const V&) const>(&C::operator/)));

	template<typename C, typename R>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T, T>(0)) == sizeof(yes) || sizeof(test<T, V>(0)) == sizeof(yes))
	};
};

/*
 * Logical operators
 */

template<typename T, typename V>
class has_eq_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<bool (C::*)(const V&) const>(&C::operator==)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T>(0)) == sizeof(yes))
	};
};

template<typename T, typename V>
class has_neq_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<bool (C::*)(const V&) const>(&C::operator!=)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T>(0)) == sizeof(yes))
	};
};

template<typename T, typename V>
class has_lt_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<bool (C::*)(const V&) const>(&C::operator<)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T>(0)) == sizeof(yes))
	};
};

template<typename T, typename V>
class has_lte_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<bool (C::*)(const V&) const>(&C::operator<=)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T>(0)) == sizeof(yes))
	};
};

template<typename T, typename V>
class has_gt_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<bool (C::*)(const V&) const>(&C::operator>)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T>(0)) == sizeof(yes))
	};
};

template<typename T, typename V>
class has_gte_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<bool (C::*)(const V&) const>(&C::operator>=)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T>(0)) == sizeof(yes))
	};
};

template<typename T>
class has_minus_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<C (C::*)(void) const>(&C::operator-)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T>(0)) == sizeof(yes))
	};
};

template<typename T>
class has_negate_operator
{
	using yes = char;
	struct no
	{
		char x[2];
	};

	template<typename C>
	static yes test(decltype(static_cast<C (C::*)(void) const>(&C::operator!)));

	template<typename C>
	static no test(...);

public:
	enum
	{
		value = (sizeof(test<T>(0)) == sizeof(yes))
	};
};
}  // namespace selector
