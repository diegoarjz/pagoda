#include "construction_argument_not_found.h"

namespace pagoda
{
ConstructionArgumentNotFound::ConstructionArgumentNotFound(const std::string nodeName, const uint32_t nodeId,
                                                           const std::string &argName)
    : Exception("Construction argument " + argName + " not found for node " + nodeName +
                " (id: " + std::to_string(nodeId) + ")")
{
}
}  // namespace pagoda
