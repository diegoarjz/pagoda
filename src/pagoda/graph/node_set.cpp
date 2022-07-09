#include "pagoda/graph/node_set.h"

#include "pagoda/graph/node.h"
#include "pagoda/graph/query/query.h"

#include <pagoda/common/debug/assertions.h>

namespace pagoda::graph
{
bool NodePtrCompare::operator()(const NodePtr &lhs, const NodePtr &rhs) const { return lhs->GetId() < rhs->GetId(); }

void NodeSet::ForEach(std::function<void(NodePtr)> handle)
{
	for (const NodePtr &n : (*this)) {
		handle(n);
	}
}

void NodeSet::ForEachUntil(std::function<bool(NodePtr)> handle)
{
	for (const NodePtr &n : (*this)) {
		if (!handle(n)) {
			return;
		}
	}
}

void NodeSet::ExecuteQuery(query::Query &q)
{
	for (const NodePtr &n : (*this)) {
		if (q.Matches(n)) {
			q.AddNode(n);
		}
	}
}
}  // namespace pagoda::graph
