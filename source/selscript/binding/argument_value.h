#pragma once

#include "../value/base_value.h"

#include <iostream>

namespace sscript
{
template<typename T, typename V>
struct argument_value_helper
{
	static T get(const V&);
};

template<typename T, typename V>
T argument_value(const V& v)
{
	return argument_value_helper<T, V>::get(v);
}

template<>
bool argument_value<bool, BaseValuePtr>(const BaseValuePtr&);

template<>
float argument_value<float, BaseValuePtr>(const BaseValuePtr&);

template<>
int argument_value<int, BaseValuePtr>(const BaseValuePtr&);

template<>
std::string argument_value<std::string, BaseValuePtr>(const BaseValuePtr&);

template<>
const BaseValuePtr& argument_value<const BaseValuePtr&, BaseValuePtr>(const BaseValuePtr&);

}  // namespace sscript
