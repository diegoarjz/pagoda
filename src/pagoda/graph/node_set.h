#ifndef PAGODA_PROCEDURAL_GRAPH_NODE_SET_H_
#define PAGODA_PROCEDURAL_GRAPH_NODE_SET_H_

#include <functional>
#include <memory>
#include <numeric>
#include <set>

namespace pagoda::graph
{
namespace query
{
class Query;
}

class Node;
using NodePtr = std::shared_ptr<Node>;
using NodeWeakPtr = std::weak_ptr<Node>;

struct NodePtrCompare
{
	bool operator()(const NodePtr &lhs, const NodePtr &rhs) const;
};

class NodeSet : public std::set<NodePtr, NodePtrCompare>
{
	public:
	void ForEach(std::function<void(NodePtr)> handle);
	void ForEachUntil(std::function<bool(NodePtr)> handle);
	void ExecuteQuery(query::Query &q);

	private:
};
}  // namespace pagoda::graph

#endif
