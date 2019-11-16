#include "unknown_operation.h"

namespace selector
{
UnknownOperation::UnknownOperation(const std::string operationName) : Exception("Unknown operation " + operationName) {}
}  // namespace selector
