#pragma once
#ifndef PAGODA_PROCEDURAL_GRAPH_OPERATION_NODE_H_
#define PAGODA_PROCEDURAL_GRAPH_OPERATION_NODE_H_

#include "node.h"

namespace pagoda
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

	void SetConstructionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &) override;

	void Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) override;
	void SetOperation(ProceduralOperationPtr operation);
	ProceduralOperationPtr GetOperation() const { return m_operation; }
	void AcceptNodeVisitor(NodeVisitor *visitor) override;

private:
	ProceduralOperationPtr m_operation;
	OperationFactoryPtr m_operationFactory;
};  // class OperationNode
}  // namespace pagoda

#endif
