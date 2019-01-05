#include "output_interface_execution.h"

#include "graph.h"
#include "graph_execution_context.h"
#include "node.h"
#include "node_execution_result.h"
#include "operation_execution.h"

#include <procedural_objects/procedural_operation.h>

namespace selector
{
class OutputInterfaceExecution::Impl
{
public:
	Impl() : m_interfaceName(0, 0) {}

	NodeExecutionResultPtr Execute(NodePtr node, GraphExecutionContextPtr executionContext)
	{
		START_PROFILE;

		m_proceduralObjects.clear();

		std::list<NodePtr> outputInterfaceNodes =
		    executionContext->m_graph->GetNodeOutputNodesByType(node, NodeType::InputInterface);
		std::list<NodePtr> operationNodes =
		    executionContext->m_graph->GetNodeInputNodesByType(node, NodeType::Operation);
		NodePtr operationNode = operationNodes.front();

		auto operationNodeResult = operationNode->GetResult()->GetResultAs<OperationExecution::Result>();

		ProceduralObjectPtr proceduralObject = nullptr;
		while ((proceduralObject = operationNodeResult->m_operation->PopProceduralObject(m_interfaceName)))
		{
			m_proceduralObjects.push_back(proceduralObject);
		}

		auto result = std::make_shared<NodeExecutionResult>(NodeExecutionResult::ResultStatus::Success);
		Result* resultPayload = new Result;
		resultPayload->m_interfaceName = &m_interfaceName;
		resultPayload->m_proceduralObjects = &m_proceduralObjects;
		result->SetResultAs<Result>(resultPayload);

		return result;
	}

	void SetInterfaceName(const InterfaceName& name) { m_interfaceName = name; }
	const InterfaceName& GetInterfaceName() { return m_interfaceName; }

private:
	InterfaceName m_interfaceName;
	std::list<ProceduralObjectPtr> m_proceduralObjects;
};  // class NodeExecution::Impl

OutputInterfaceExecution::OutputInterfaceExecution()
    : m_implementation(std::make_unique<OutputInterfaceExecution::Impl>())
{
}

OutputInterfaceExecution::~OutputInterfaceExecution() {}

NodeExecutionResultPtr OutputInterfaceExecution::Execute(NodePtr node, GraphExecutionContextPtr executionContext)
{
	return m_implementation->Execute(node, executionContext);
}

void OutputInterfaceExecution::SetInterfaceName(const InterfaceName& name) { m_implementation->SetInterfaceName(name); }
const InterfaceName& OutputInterfaceExecution::GetInterfaceName() const { return m_implementation->GetInterfaceName(); }

}  // namespace selector
