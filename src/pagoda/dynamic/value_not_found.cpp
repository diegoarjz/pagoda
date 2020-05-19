#include "value_not_found.h"

namespace pagoda::dynamic
{
ValueNotFoundException::ValueNotFoundException(const std::string &valueName)
    : common::exception::Exception("Value with name '" + valueName + "' not found in value table")
{
}
}  // namespace pagoda::dynamic
