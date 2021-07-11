#include "construction_argument_not_found.h"

namespace pagoda::graph
{
ConstructionArgumentNotFound::ConstructionArgumentNotFound(const std::string nodeName, const std::string &argName)
  : Exception("Construction argument " + argName + " not found for node " + nodeName)
{
}
}  // namespace pagoda::graph
