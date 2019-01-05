#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_EXECUTION_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_EXECUTION_H_

#include <memory>

namespace selector
{
struct GraphExecutionContext;
using GraphExecutionContextPtr = std::shared_ptr<GraphExecutionContext>;
class Node;
using NodePtr = std::shared_ptr<Node>;
class NodeExecutionResult;
using NodeExecutionResultPtr = std::shared_ptr<NodeExecutionResult>;

class NodeExecution
{
public:
	virtual ~NodeExecution();
	virtual NodeExecutionResultPtr Execute(NodePtr node, GraphExecutionContextPtr executionContext) = 0;

};  // class NodeExecution
}  // namespace selector
#endif
