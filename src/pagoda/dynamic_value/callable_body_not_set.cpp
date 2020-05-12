#include "callable_body_not_set.h"

namespace pagoda
{
CallableBodyNotSet::CallableBodyNotSet(const std::string &dynamicValueName)
    : common::exception::Exception("Callable body not set in " + dynamicValueName)
{
}
}  // namespace pagoda
