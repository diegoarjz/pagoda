#include "default_scheduler.h"

#include "breadth_first_node_visitor.h"

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
	nextNode->Execute(inNodes, outNodes);

	return true;
}

void DefaultScheduler::Finalize() {}
}  // namespace selector
