#include "argument_value.h"

#include "../value/boolean_value.h"
#include "../value/float_value.h"
#include "../value/integer_value.h"
#include "../value/string_value.h"
#include "../value/value_visitor.h"

namespace sscript
{
template<>
bool argument_value<bool, BaseValuePtr>(const BaseValuePtr& v)
{
	return value_as<Boolean>(v).m_value;
}

template<>
float argument_value<float, BaseValuePtr>(const BaseValuePtr& v)
{
	return value_as<Float>(v).m_value;
}

template<>
int argument_value<int, BaseValuePtr>(const BaseValuePtr& v)
{
	return value_as<Integer>(v).m_value;
}

template<>
std::string argument_value<std::string, BaseValuePtr>(const BaseValuePtr& v)
{
	return value_as<String>(v).m_value;
}

template<>
const BaseValuePtr& argument_value<const BaseValuePtr&, BaseValuePtr>(const BaseValuePtr& v)
{
	return v;
}
}  // namespace sscript
