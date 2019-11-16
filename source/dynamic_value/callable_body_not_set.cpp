#include "callable_body_not_set.h"

namespace selector
{
CallableBodyNotSet::CallableBodyNotSet(const std::string &dynamicValueName)
    : Exception("Callable body not set in " + dynamicValueName)
{
}
}  // namespace selector
