#include "value_not_found.h"

namespace pagoda
{
ValueNotFoundException::ValueNotFoundException(const std::string &valueName)
    : Exception("Value with name '" + valueName + "' not found in value table")
{
}
}  // namespace pagoda
