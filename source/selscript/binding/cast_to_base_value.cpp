#include "cast_to_base_value.h"

#include "../value/integer_value.h"

namespace sscript
{
BaseValuePtr cast_to_base_value<int>::cast(int&& from) { return std::make_shared<Integer>(from); }

}  // namespace sscript
