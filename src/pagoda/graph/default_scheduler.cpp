#include "default_scheduler.h"

#include "input_interface_node.h"
#include "operation_node.h"
#include "output_interface_node.h"
#include "query/type.h"

#include <pagoda/common/exception/exception.h>
#include <pagoda/common/instrument/profiler.h>
#include <ctime>
#include <memory>

namespace pagoda::graph
{
DefaultScheduler::DefaultScheduler(Graph &graph) : m_graph(graph), m_executionQueue(m_graph) {}

void DefaultScheduler::Initialize() 
{
	query::Type<InputInterfaceNode> q(m_graph, [&](NodePtr n) {
		auto inputInterface = std::dynamic_pointer_cast<InputInterfaceNode>(n);

		std::vector<std::shared_ptr<OperationNode>> downstreamNodes;

		for (const auto &outNode : m_graph.GetNodeOutputNodes(n->GetName()))
		{
			if (auto op = std::dynamic_pointer_cast<OperationNode>(outNode))
			{
				downstreamNodes.push_back(op);
			}
		}
		for (const auto &inNode : m_graph.GetNodeInputNodes(n->GetName()))
		{
			if (auto outInterface = std::dynamic_pointer_cast<OutputInterfaceNode>(inNode))
			{
				for (const auto &inOp : m_graph.GetNodeInputNodes(outInterface->GetName()))
				{
					if (auto op = std::dynamic_pointer_cast<OperationNode>(inOp))
					{
						for (auto &downStream : downstreamNodes)
						{
							downStream->GetOperation()->LinkInputInterface(inputInterface->GetInterfaceName(),
							                                               outInterface->GetInterfaceName(),
							                                               op->GetOperation());

							std::cout << "linking " << op->GetName() << "(" << inputInterface->GetInterfaceName()
							          << ") -> " << downStream->GetName() << "(" << outInterface->GetInterfaceName()
							          << ")" << std::endl;
						}
					}
				}
			}
		}
	});
	m_graph.ExecuteQuery(q);
}

bool DefaultScheduler::Step()
{
	START_PROFILE;

	auto nextNode = m_executionQueue.GetNextNode();
	if (nextNode == nullptr)
	{
		return false;
	}

	auto inNodes = m_graph.GetNodeInputNodes(nextNode->GetName());
	auto outNodes = m_graph.GetNodeOutputNodes(nextNode->GetName());

	nextNode->SetExpressionVariables();
	try
	{
		LOG_INFO("Executing node '" << nextNode->GetName() << "'");
		nextNode->Execute(inNodes, outNodes);
	}
	catch (common::exception::Exception &e)
	{
		LOG_ERROR("Exception caught while executing Node " << nextNode->GetName() << "(" << nextNode->GetId() << ")");
		LOG_ERROR(e.What());
	}
	catch (...)
	{
		LOG_FATAL("Unknown exception caught while executing Node " << nextNode->GetName() << "(" << nextNode->GetId()
		                                                           << ")");
		throw;
	}

	return true;
}

void DefaultScheduler::Finalize() {}
}  // namespace pagoda::graph
