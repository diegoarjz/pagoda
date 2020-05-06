#include "unknown_operation.h"

namespace pagoda
{
UnknownOperation::UnknownOperation(const std::string operationName) : Exception("Unknown operation " + operationName) {}
}  // namespace pagoda
