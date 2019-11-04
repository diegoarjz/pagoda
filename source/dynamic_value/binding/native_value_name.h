#pragma once

#include <string>
#include <typeinfo>

namespace selector
{
template<typename C>
struct native_value_name
{
	static const std::string GetName() { return typeid(C).name(); }
};

template<>
const std::string native_value_name<char>::GetName();

template<>
const std::string native_value_name<int>::GetName();

template<>
const std::string native_value_name<float>::GetName();

template<>
const std::string native_value_name<bool>::GetName();

template<>
const std::string native_value_name<std::string>::GetName();
}  // namespace selector
