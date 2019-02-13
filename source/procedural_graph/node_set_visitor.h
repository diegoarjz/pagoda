#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_SET_VISITOR_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_SET_VISITOR_H_

#include "node.h"
#include "node_set.h"

namespace selector
{
template<class T>
void node_type_filter(const NodeSet<Node> &nodeSet, NodeSet<T> &outNodes)
{
	for (auto node : nodeSet)
	{
		auto ptr = std::dynamic_pointer_cast<T>(node);
		if (ptr)
		{
			outNodes.insert(ptr);
		}
	}
}
}  // namespace selector

#endif
