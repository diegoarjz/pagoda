#ifndef SELECTOR_PROCEDURAL_GRAPH_DEFAULT_SCHEDULER_H_
#define SELECTOR_PROCEDURAL_GRAPH_DEFAULT_SCHEDULER_H_

#include "execution_queue.h"
#include "graph.h"
#include "node.h"

#include "scheduler.h"

#include <queue>
#include <unordered_set>

namespace selector
{
class DefaultScheduler : public IScheduler
{
public:
	DefaultScheduler(Graph &graph);

	void Initialize() override;
	bool Step() override;
    void Finalize() override;

private:
	using NodeWeakPtrSet = std::unordered_set<NodeWeakPtr, NodeWeakPtrHasher, NodeWeakPtrEqual>;

	Graph &m_graph;
	ExecutionQueue m_executionQueue;
};  // class Scheduler
}  // namespace selector
#endif

