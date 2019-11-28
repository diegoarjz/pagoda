#include "node_set.h"

#include "node.h"

#include "common/assertions.h"

namespace selector
{
bool NodePtrCompare::operator()(const NodePtr &lhs, const NodePtr &rhs) const
{
	DBG_ASSERT_MSG(lhs->GetId() != rhs->GetId() || lhs != rhs);
	return lhs->GetId() < rhs->GetId();
}
}  // namespace selector
