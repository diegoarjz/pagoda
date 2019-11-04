#pragma once

#include "../dynamic_value_base.h"

#include "native_value_name.h"

namespace selector
{
/**
 * Returns a human readable name for the type T.
 *
 * If T is a \c DynamicValueBase then it uses its \c TypeInfo.
 * If T is a native type then it uses the \c native_value_name<T>.
 */
template<typename T>
struct type_name
{
	template<typename U = T>
	static typename std::enable_if<std::is_base_of<DynamicValueBase, U>::value, std::string>::type GetName()
	{
		return U::s_typeInfo->GetTypeName();
	}

	template<typename U = T>
	static typename std::enable_if<!std::is_base_of<DynamicValueBase, U>::value, std::string>::type GetName()
	{
		return native_value_name<U>::GetName();
	}
};
}  // namespace selector
