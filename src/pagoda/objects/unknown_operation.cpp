#include "unknown_operation.h"

namespace pagoda::objects
{
UnknownOperation::UnknownOperation(const std::string operationName)
  : common::exception::Exception("Unknown operation " + operationName)
{
}
}  // namespace pagoda::objects
