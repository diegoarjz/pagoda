#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_SET_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_SET_H_

#include <unordered_set>

namespace selector
{
template<class NodeT>
using NodeSet = std::unordered_set<std::shared_ptr<NodeT>>;
}  // namespace selector

#endif
