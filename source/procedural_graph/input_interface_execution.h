#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_INPUT_INTERFACE_EXECUTION_H_
#define SELECTOR_PROCEDURAL_GRAPH_INPUT_INTERFACE_EXECUTION_H_

#include "node_execution.h"

#include <list>

namespace selector
{
struct InterfaceName;

class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class NodeExecutionResult;
using NodeExecutionResultPtr = std::shared_ptr<NodeExecutionResult>;
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;

class InputInterfaceExecution : public NodeExecution
{
public:
	struct Result
	{
		const std::list<ProceduralObjectPtr>* m_proceduralObjects;
		const InterfaceName* m_interfaceName;
	};

	InputInterfaceExecution();
	~InputInterfaceExecution();
	NodeExecutionResultPtr Execute(NodePtr node, GraphExecutionContextPtr graph) override;
	void SetInterfaceName(const InterfaceName& interfaceName);
	const InterfaceName& GetInterfaceName() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};  // class OperationExecution
}  // namespace selector

#endif
