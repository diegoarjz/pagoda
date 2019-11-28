#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_PARAMETER_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_PARAMETER_NODE_H_

#include "node.h"

namespace selector
{
/**
 * Node to propagate parameters to its out nodes.
 * All parameters in defined in this node are set in the parameter context of each of the
 * output nodes overwriting them.
 */
class ParameterNode : public Node
{
public:
	static const char *name;

	ParameterNode();
	virtual ~ParameterNode();

	void SetConstructionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &) override;
	void AcceptNodeVisitor(NodeVisitor *visitor) override;

	void Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) override;
};

}  // namespace selector

#endif
