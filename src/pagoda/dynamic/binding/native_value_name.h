#pragma once

#include <string>
#include <typeinfo>

namespace pagoda::dynamic
{
template<typename C>
struct native_value_name
{
	inline static const std::string GetName() { return typeid(C).name(); }
};

template<>
inline const std::string native_value_name<char>::GetName()
{
	return "char";
}

template<>
inline const std::string native_value_name<int>::GetName()
{
	return "int";
}

template<>
inline const std::string native_value_name<float>::GetName()
{
	return "float";
}

template<>
inline const std::string native_value_name<bool>::GetName()
{
	return "bool";
}

template<>
inline const std::string native_value_name<std::string>::GetName()
{
	return "std::string";
}
}  // namespace pagoda::dynamic
