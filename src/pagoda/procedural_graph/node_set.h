#ifndef PAGODA_PROCEDURAL_GRAPH_NODE_SET_H_
#define PAGODA_PROCEDURAL_GRAPH_NODE_SET_H_

#include <memory>
#include <set>

namespace pagoda
{
class Node;
using NodePtr = std::shared_ptr<Node>;

struct NodePtrCompare
{
	bool operator()(const NodePtr &lhs, const NodePtr &rhs) const;
};

template<class NodeT>
using NodeSet = std::set<std::shared_ptr<NodeT>, NodePtrCompare>;
}  // namespace pagoda

#endif
