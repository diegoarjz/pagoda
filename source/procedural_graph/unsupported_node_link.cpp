#include "unsupported_node_link.h"

namespace selector
{
UnsupportedNodeLink::UnsupportedNodeLink(const std::string &direction, const std::string nodeType)
    : Exception("Unsupported " + direction + " node link for " + nodeType + " node type")
{
}
}  // namespace selector
