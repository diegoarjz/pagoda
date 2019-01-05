#include "input_interface_execution.h"

#include "graph.h"
#include "graph_execution_context.h"
#include "node.h"
#include "node_execution_result.h"
#include "output_interface_execution.h"

#include <procedural_objects/procedural_operation.h>

namespace selector
{
class InputInterfaceExecution::Impl
{
public:
	Impl() : m_interfaceName(0, 0) {}

	NodeExecutionResultPtr Execute(NodePtr node, GraphExecutionContextPtr executionContext)
	{
		START_PROFILE;

		m_proceduralObjects.clear();

		std::list<NodePtr> outputInterfaceNodes =
		    executionContext->m_graph->GetNodeInputNodesByType(node, NodeType::OutputInterface);

		for (NodePtr outNode : outputInterfaceNodes)
		{
			auto outNodeResult = outNode->GetResult()->GetResultAs<OutputInterfaceExecution::Result>();

			const std::list<ProceduralObjectPtr>* outNodeObjects = outNodeResult->m_proceduralObjects;

			m_proceduralObjects.insert(std::end(m_proceduralObjects), std::begin(*outNodeObjects),
			                           std::end(*outNodeObjects));
		}

		auto result = std::make_shared<NodeExecutionResult>(NodeExecutionResult::ResultStatus::Success);
		Result* resultPayload = new Result;
		resultPayload->m_interfaceName = &m_interfaceName;
		resultPayload->m_proceduralObjects = &m_proceduralObjects;
		result->SetResultAs<Result>(resultPayload);

		return result;
	}

	void SetInterfaceName(const InterfaceName& interfaceName) { m_interfaceName = interfaceName; }
	const InterfaceName& GetInterfaceName() const { return m_interfaceName; }

private:
	InterfaceName m_interfaceName;
	std::list<ProceduralObjectPtr> m_proceduralObjects;
};  // class NodeExecution::Impl

InputInterfaceExecution::InputInterfaceExecution() : m_implementation(std::make_unique<InputInterfaceExecution::Impl>())
{
}

InputInterfaceExecution::~InputInterfaceExecution() {}

NodeExecutionResultPtr InputInterfaceExecution::Execute(NodePtr node, GraphExecutionContextPtr executionContext)
{
	return m_implementation->Execute(node, executionContext);
}

void InputInterfaceExecution::SetInterfaceName(const InterfaceName& interfaceName)
{
	m_implementation->SetInterfaceName(interfaceName);
}

const InterfaceName& InputInterfaceExecution::GetInterfaceName() const { return m_implementation->GetInterfaceName(); }

}  // namespace selector
