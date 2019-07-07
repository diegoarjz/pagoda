#include "parameter.h"
#include <iostream>

namespace selector
{
template<>
template<>
float parameter_getter<float>::operator()<ExpressionPtr>(const ExpressionPtr &e)
{
	return e->GetAsFloat();
}

template<>
template<>
std::string parameter_getter<std::string>::operator()<ExpressionPtr>(const ExpressionPtr &e)
{
	return e->GetAsString();
}

}  // namespace selector
