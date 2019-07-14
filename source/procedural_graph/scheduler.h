#ifndef SELECTOR_PROCEDURAL_GRAPH_SCHEDULER_H_
#define SELECTOR_PROCEDURAL_GRAPH_SCHEDULER_H_

#include "execution_queue.h"
#include "graph.h"
#include "node.h"

#include "parameter/context.h"

#include <queue>
#include <unordered_set>

namespace selector
{
struct GraphExecutionContext;
using GraphExecutionContextPtr = std::shared_ptr<GraphExecutionContext>;

class Scheduler
{
public:
	Scheduler(GraphExecutionContextPtr executionContext, std::shared_ptr<Context> &context);

	void Initialize();
	bool Step();

private:
	using NodeWeakPtrSet = std::unordered_set<NodeWeakPtr, NodeWeakPtrHasher, NodeWeakPtrEqual>;

	std::shared_ptr<GraphExecutionContext> m_graphExecutionContext;
	std::shared_ptr<Graph> m_graph;
	std::shared_ptr<Context> m_graphParameterContext;
	ExecutionQueue m_executionQueue;
};  // class Scheduler
}  // namespace selector
#endif
