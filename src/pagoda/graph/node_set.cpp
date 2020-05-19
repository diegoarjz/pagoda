#include "node_set.h"

#include "node.h"

#include <pagoda/common/debug/assertions.h>

namespace pagoda::graph
{
bool NodePtrCompare::operator()(const NodePtr &lhs, const NodePtr &rhs) const { return lhs->GetId() < rhs->GetId(); }
}  // namespace pagoda::graph
