#include "unknown_node_type.h"

namespace pagoda
{
UnknownNodeTypeException::UnknownNodeTypeException(const std::string &nodeType)
    : Exception("Unknown Node Type " + nodeType)
{
}
}  // namespace pagoda
