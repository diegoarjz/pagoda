#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_OPERATION_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_OPERATION_NODE_H_

#include "node.h"

namespace selector
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class OperationFactory;
using OperationFactoryPtr = std::shared_ptr<OperationFactory>;

class OperationNode : public Node
{
public:
	static const char *name;

	OperationNode(OperationFactoryPtr operationFactory);
	~OperationNode();

	void SetConstructionArguments(const std::unordered_map<std::string, Parameter> &) override;

	void Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) override;
	void SetOperation(ProceduralOperationPtr operation);
	ProceduralOperationPtr GetOperation() const { return m_operation; }

private:
	ProceduralOperationPtr m_operation;
    OperationFactoryPtr m_operationFactory;
};  // class OperationNode
}  // namespace selector

#endif
