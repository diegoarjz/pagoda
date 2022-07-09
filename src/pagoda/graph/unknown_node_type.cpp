#include "pagoda/graph/unknown_node_type.h"

namespace pagoda::graph
{
UnknownNodeTypeException::UnknownNodeTypeException(const std::string &nodeType)
  : common::exception::Exception("Unknown Node Type " + nodeType)
{
}
}  // namespace pagoda::graph
