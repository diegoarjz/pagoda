#include "default_scheduler.h"

#include "breadth_first_node_visitor.h"

#include <pagoda/common/exception/exception.h>
#include <pagoda/common/instrument/profiler.h>

namespace pagoda::graph
{
DefaultScheduler::DefaultScheduler(Graph &graph) : m_graph(graph), m_executionQueue(m_graph) {}

void DefaultScheduler::Initialize() {}

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
