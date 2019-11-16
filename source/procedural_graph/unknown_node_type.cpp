#include "unknown_node_type.h"

namespace selector
{
UnknownNodeTypeException::UnknownNodeTypeException(const std::string &nodeType)
    : Exception("Unknown Node Type " + nodeType)
{
}
}  // namespace selector
