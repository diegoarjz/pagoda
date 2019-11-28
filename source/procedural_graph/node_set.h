#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_SET_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_SET_H_

#include <memory>
#include <set>

namespace selector
{
class Node;
using NodePtr = std::shared_ptr<Node>;

struct NodePtrCompare
{
	bool operator()(const NodePtr &lhs, const NodePtr &rhs) const;
};

template<class NodeT>
using NodeSet = std::set<std::shared_ptr<NodeT>, NodePtrCompare>;
}  // namespace selector

#endif
