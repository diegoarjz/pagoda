#include "node_set.h"

#include "node.h"

#include "common/assertions.h"

namespace selector
{
bool NodePtrCompare::operator()(const NodePtr &lhs, const NodePtr &rhs) const { return lhs->GetId() < rhs->GetId(); }
}  // namespace selector
