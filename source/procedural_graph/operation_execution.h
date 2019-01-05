#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_OPERATION_EXECUTION_H_
#define SELECTOR_PROCEDURAL_GRAPH_OPERATION_EXECUTION_H_

#include "node_execution.h"

namespace selector
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class NodeExecutionResult;
using NodeExecutionResultPtr = std::shared_ptr<NodeExecutionResult>;

class OperationExecution : public NodeExecution
{
public:
	struct Result
	{
		ProceduralOperationPtr m_operation;
	};

	OperationExecution();
	~OperationExecution();
	NodeExecutionResultPtr Execute(NodePtr node, GraphExecutionContextPtr graph) override;
	void SetOperation(ProceduralOperationPtr operation);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};  // class OperationExecution
}  // namespace selector

#endif
