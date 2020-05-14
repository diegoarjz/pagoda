#pragma once
#ifndef PAGODA_PROCEDURAL_GRAPH_OPERATION_NODE_H_
#define PAGODA_PROCEDURAL_GRAPH_OPERATION_NODE_H_

#include "node.h"

namespace pagoda
{
namespace objects
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class OperationFactory;
using OperationFactoryPtr = std::shared_ptr<OperationFactory>;
}  // namespace objects

class OperationNode : public Node
{
public:
	static const char *name;

	OperationNode(objects::OperationFactoryPtr operationFactory);
	~OperationNode();

	void SetConstructionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &) override;

	void Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) override;
	void SetOperation(objects::ProceduralOperationPtr operation);
	objects::ProceduralOperationPtr GetOperation() const { return m_operation; }
	void AcceptNodeVisitor(NodeVisitor *visitor) override;

private:
	objects::ProceduralOperationPtr m_operation;
	objects::OperationFactoryPtr m_operationFactory;
};  // class OperationNode
}  // namespace pagoda

#endif
