#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_OUTPUT_INTERFACE_EXECUTION_H_
#define SELECTOR_PROCEDURAL_GRAPH_OUTPUT_INTERFACE_EXECUTION_H_

#include "node_execution.h"

#include <list>

namespace selector
{
struct InterfaceName;
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;

class OutputInterfaceExecution : public NodeExecution
{
public:
	struct Result
	{
		const std::list<ProceduralObjectPtr>* m_proceduralObjects;
		const InterfaceName* m_interfaceName;
	};

	OutputInterfaceExecution();
	~OutputInterfaceExecution();
	NodeExecutionResultPtr Execute(NodePtr node, GraphExecutionContextPtr graph) override;
	void SetInterfaceName(const InterfaceName& name);
	const InterfaceName& GetInterfaceName() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};  // class OperationExecution
}  // namespace selector

#endif
