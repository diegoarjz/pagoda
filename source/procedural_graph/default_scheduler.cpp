#include "default_scheduler.h"

#include "breadth_first_node_visitor.h"
#include "common/exception.h"
#include "common/profiler.h"

namespace selector
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

	auto inNodes = m_graph.GetNodeInputNodes(nextNode);
	auto outNodes = m_graph.GetNodeOutputNodes(nextNode);

	nextNode->SetExpressionVariables();
	try
	{
		LOG_INFO("Executing node '" << nextNode->GetName() << "'");
		nextNode->Execute(inNodes, outNodes);
	}
	catch (Exception &e)
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
}  // namespace selector
