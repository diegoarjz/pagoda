#include "callable_body_not_set.h"

namespace pagoda::dynamic
{
CallableBodyNotSet::CallableBodyNotSet(const std::string &dynamicValueName)
  : common::exception::Exception("Callable body not set in " + dynamicValueName)
{
}
}  // namespace pagoda::dynamic
