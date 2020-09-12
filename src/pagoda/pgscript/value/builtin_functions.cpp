#include "builtin_functions.h"

#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/integer_value.h>

#include <chrono>
#include <iostream>

namespace pagoda
{
using namespace dynamic;

void print(const std::vector<DynamicValueBasePtr> &args)
{
	for (auto &a : args) {
		std::cout << a->ToString();
	}
	std::cout << std::endl;
}

TypeInfoPtr type(const DynamicValueBasePtr &value) { return value->GetTypeInfo(); }
}  // namespace pagoda
