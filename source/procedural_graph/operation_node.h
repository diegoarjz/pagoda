#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_OPERATION_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_OPERATION_NODE_H_

#include "node.h"

namespace selector
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;

class OperationNode : public Node::Registrar<OperationNode>
{
public:
	static const char *name;

	OperationNode();
	~OperationNode();
	void Execute(GraphExecutionContextPtr graph, const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) override;
	void SetOperation(ProceduralOperationPtr operation);
	ProceduralOperationPtr GetOperation() const { return m_operation; }

private:
	ProceduralOperationPtr m_operation;
};  // class OperationNode
}  // namespace selector

#endif
