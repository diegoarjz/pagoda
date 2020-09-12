#pragma once
#ifndef PAGODA_PROCEDURAL_GRAPH_OPERATION_NODE_H_
#define PAGODA_PROCEDURAL_GRAPH_OPERATION_NODE_H_

#include "node.h"

namespace pagoda::objects
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class OperationFactory;
using OperationFactoryPtr = std::shared_ptr<OperationFactory>;
}  // namespace pagoda::objects

namespace pagoda::graph
{
class OperationNode : public Node
{
	public:
	static const char *name;

	OperationNode(pagoda::objects::OperationFactoryPtr operationFactory);
	~OperationNode();

	void SetConstructionArguments(const std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &) override;

	void Execute(const NodeSet &inNodes, const NodeSet &outNodes) override;
	void SetOperation(pagoda::objects::ProceduralOperationPtr operation);
	pagoda::objects::ProceduralOperationPtr GetOperation() const { return m_operation; }
	void AcceptNodeVisitor(NodeVisitor *visitor) override;

	private:
	pagoda::objects::ProceduralOperationPtr m_operation;
	pagoda::objects::OperationFactoryPtr m_operationFactory;
};  // class OperationNode
}  // namespace pagoda::graph

#endif
