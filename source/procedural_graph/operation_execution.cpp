#include "operation_execution.h"

#include "graph.h"
#include "graph_execution_context.h"
#include "input_interface_execution.h"
#include "node.h"
#include "node_execution_result.h"

#include <procedural_objects/procedural_operation.h>

namespace selector
{
class OperationExecution::Impl
{
public:
	NodeExecutionResultPtr Execute(NodePtr node, GraphExecutionContextPtr executionContext)
	{
		std::shared_ptr<Context> paramContext = node->GetParameterContext();
		std::list<NodePtr> inputInterafaceNodes =
		    executionContext->m_graph->GetNodeInputNodesByType(node, NodeType::InputInterface);

		for (auto param : paramContext->GetParameters())
		{
			auto parameter = param.second;
			if (parameter->GetExpression() != nullptr)
			{
				parameter->GetExpression()->Evaluate();
			}
		}

		for (auto inInterface : inputInterafaceNodes)
		{
			NodeExecutionResultPtr executionResult = inInterface->GetResult();
			auto* result = executionResult->GetResultAs<InputInterfaceExecution::Result>();

			for (ProceduralObjectPtr o : *(result->m_proceduralObjects))
			{
				m_operation->PushProceduralObject(*result->m_interfaceName, o);
			}
		}

		// TODO: Is it really necessary to always create an OperationExecutionContext?
		auto ctx = std::make_shared<OperationExecutionContext>();
		ctx->geometry_system = executionContext->m_geometrySystem.lock();
		ctx->procedural_object_system = executionContext->m_proceduralObjectSystem.lock();
		ctx->parameter_context = paramContext;

		m_operation->SetExecutionContext(ctx);
		m_operation->Execute();

		auto nodeResult = std::make_shared<NodeExecutionResult>(NodeExecutionResult::ResultStatus::Success);
		auto operationResult = new Result;
		operationResult->m_operation = m_operation;
		nodeResult->SetResultAs<Result>(operationResult);

		return nodeResult;
	}

	void SetOperation(ProceduralOperationPtr operation) { m_operation = operation; }

private:
	ProceduralOperationPtr m_operation;
};  // class NodeExecution::Impl

OperationExecution::OperationExecution() : m_implementation(std::make_unique<OperationExecution::Impl>()) {}
OperationExecution::~OperationExecution() {}

NodeExecutionResultPtr OperationExecution::Execute(NodePtr node, GraphExecutionContextPtr executionContext)
{
	return m_implementation->Execute(node, executionContext);
}

void OperationExecution::SetOperation(ProceduralOperationPtr operation) { m_implementation->SetOperation(operation); }
}  // namespace selector
