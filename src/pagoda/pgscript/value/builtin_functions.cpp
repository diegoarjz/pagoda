#include "builtin_functions.h"

#include "../../dynamic_value/float_value.h"
#include "../../dynamic_value/integer_value.h"

#include <chrono>
#include <iostream>

namespace pagoda
{
void print(const std::vector<DynamicValueBasePtr> &args)
{
	for (auto &a : args)
	{
		std::cout << a->ToString();
	}
	std::cout << std::endl;
}

TypeInfoPtr type(const DynamicValueBasePtr &value) { return value->GetTypeInfo(); }
}  // namespace pagoda
