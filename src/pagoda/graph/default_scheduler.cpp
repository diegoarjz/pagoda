#include "default_scheduler.h"

#include "input_interface_node.h"
#include "operation_node.h"
#include "output_interface_node.h"
#include "parameter_node.h"

#include "query/input_node.h"
#include "query/query.h"
#include "query/type.h"

#include <pagoda/common/exception/exception.h>
#include <pagoda/common/instrument/profiler.h>

#include <ctime>
#include <memory>

using namespace pagoda::objects;

namespace pagoda::graph
{
DefaultScheduler::DefaultScheduler(Graph &graph) : m_graph(graph) {}

void DefaultScheduler::Initialize()
{
	// Link the operations
	query::Type<InputInterfaceNode> q(m_graph, [&](NodePtr n) {
		auto inputInterface = std::dynamic_pointer_cast<InputInterfaceNode>(n);

		std::vector<std::shared_ptr<OperationNode>> downstreamNodes;

		for (const auto &outNode : m_graph.GetNodeOutputNodes(n->GetName())) {
			if (auto op = std::dynamic_pointer_cast<OperationNode>(outNode)) {
				downstreamNodes.push_back(op);
			}
		}
		for (const auto &inNode : m_graph.GetNodeInputNodes(n->GetName())) {
			if (auto outInterface = std::dynamic_pointer_cast<OutputInterfaceNode>(inNode)) {
				for (const auto &inOp : m_graph.GetNodeInputNodes(outInterface->GetName())) {
					if (auto op = std::dynamic_pointer_cast<OperationNode>(inOp)) {
						for (auto &downStream : downstreamNodes) {
							downStream->GetOperation()->LinkInputInterface(inputInterface->GetInterfaceName(),
							                                               outInterface->GetInterfaceName(), op->GetOperation());
						}
					}
				}
			}
		}
	});
	m_graph.ExecuteQuery(q);

	// Execute all nodes. I.e., set the parameters
	query::Query nodes(m_graph, [&](NodePtr n) {
		auto inNodes = m_graph.GetNodeInputNodes(n->GetName());
		auto outNodes = m_graph.GetNodeOutputNodes(n->GetName());
		n->Execute(inNodes, outNodes);
	});
	m_graph.ExecuteQuery(nodes);

	// Once we're done with the parameter nodes, we can destroy them
	// so that they aren't treated as input nodes (next step).
	// TODO: this should be handled better and the nodes should not be deleted
	std::list<NodePtr> paramNodes;
	query::Type<ParameterNode> parameterNodes(m_graph, [&](NodePtr n) { paramNodes.push_back(n); });
	m_graph.ExecuteQuery(parameterNodes);
	for (const auto &p : paramNodes) {
		m_graph.DestroyNode(p->GetName());
	}

	// Create the list of input nodes.
	query::InputNode inputNodes(m_graph, [&](NodePtr n) {
		auto opNode = std::dynamic_pointer_cast<OperationNode>(n);
		if (opNode != nullptr) {
			m_operationQueue.push(opNode->GetOperation().get());
		}
	});
	m_graph.ExecuteQuery(inputNodes);

	// Set the OnNeedsUpdate handler to push the operation on the queue
	query::Type<OperationNode> operationNodes(m_graph, [&](NodePtr n) {
		n->SetExpressionVariables();
		auto op = std::dynamic_pointer_cast<OperationNode>(n)->GetOperation();
		op->OnNeedsUpdate([&](ProceduralOperation *op) { m_operationQueue.push(op); });
	});
	m_graph.ExecuteQuery(operationNodes);
}

bool DefaultScheduler::Step()
{
	START_PROFILE;

	if (m_operationQueue.empty()) {
		return false;
	}

	auto nextOperation = m_operationQueue.front();
	m_operationQueue.pop();

	nextOperation->Execute();

	return true;
}

void DefaultScheduler::Finalize() {}
}  // namespace pagoda::graph
