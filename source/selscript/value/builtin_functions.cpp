#include "builtin_functions.h"

#include "float_value.h"
#include "integer_value.h"

#include <chrono>
#include <iostream>

namespace sscript
{
void print(const std::vector<BaseValuePtr> &args)
{
	for (auto &a : args)
	{
		std::cout << a->ToString();
	}
	std::cout << std::endl;
}

using clock = std::chrono::high_resolution_clock;
auto _start = clock::now().time_since_epoch();
IntegerPtr time()
{
	auto now = clock::now().time_since_epoch();
	long long milliseconds_since_start = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start).count();
	return std::make_shared<Integer>(milliseconds_since_start);
}

TypeInfoPtr type(const BaseValuePtr &value) { return value->m_type; }
}  // namespace sscript
