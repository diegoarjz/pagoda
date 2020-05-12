#include "unknown_operation.h"

namespace pagoda
{
UnknownOperation::UnknownOperation(const std::string operationName)
    : common::exception::Exception("Unknown operation " + operationName)
{
}
}  // namespace pagoda
