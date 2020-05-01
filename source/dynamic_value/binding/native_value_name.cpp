#include "native_value_name.h"

namespace pagoda
{
template<>
const std::string native_value_name<char>::GetName()
{
	return "char";
}

template<>
const std::string native_value_name<int>::GetName()
{
	return "int";
}

template<>
const std::string native_value_name<float>::GetName()
{
	return "float";
}

template<>
const std::string native_value_name<bool>::GetName()
{
	return "bool";
}

template<>
const std::string native_value_name<std::string>::GetName()
{
	return "std::string";
}
}  // namespace pagoda
