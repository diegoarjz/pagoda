#ifndef PAGODA_PROCEDURAL_GRAPH_NODE_SET_H_
#define PAGODA_PROCEDURAL_GRAPH_NODE_SET_H_

#include <memory>
#include <set>

namespace pagoda::graph
{
class Node;
using NodePtr = std::shared_ptr<Node>;

struct NodePtrCompare
{
	bool operator()(const NodePtr &lhs, const NodePtr &rhs) const;
};

class NodeSet : public std::set<NodePtr, NodePtrCompare>
{
};
}  // namespace pagoda::graph

#endif
