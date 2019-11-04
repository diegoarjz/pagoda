#include "value_not_found.h"

namespace selector
{
ValueNotFoundException::ValueNotFoundException(const std::string &valueName)
    : std::runtime_error("Value with name " + valueName + " not found in value table")
{
}
}  // namespace selector
