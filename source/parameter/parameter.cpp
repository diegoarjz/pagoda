#include "parameter.h"
#include "expression.h"

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
bool parameter_getter<bool>::operator()<std::string>(const std::string &s)
{
    if (s == "true")
    {
        return true;
    }
    else if (s == "false")
    {
        return false;
    }
    throw ParameterException("Can't convert \"" + s + "\" to bool");
}

template<>
template<>
int parameter_getter<int>::operator()<ExpressionPtr>(const std::shared_ptr<Expression> &e)
{
	return e->GetAsInt();
}

template<>
template<>
std::string parameter_getter<std::string>::operator()<ExpressionPtr>(const ExpressionPtr &e)
{
	return e->GetAsString();
}

}  // namespace selector
