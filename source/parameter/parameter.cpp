#include "parameter.h"
#include <iostream>

namespace selector
{
template<>
float parameter_getter<float>::operator()(const Expression &e) { return e.GetAsFloat(); }

template<>
std::string parameter_getter<std::string>::operator()(const Expression &e) { return e.GetAsString(); }

}  // namespace selector
