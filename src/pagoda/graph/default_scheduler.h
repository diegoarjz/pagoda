#ifndef PAGODA_PROCEDURAL_GRAPH_DEFAULT_SCHEDULER_H_
#define PAGODA_PROCEDURAL_GRAPH_DEFAULT_SCHEDULER_H_

#include "pagoda/graph/graph.h"
#include "pagoda/graph/node.h"

#include "scheduler.h"

#include <queue>

namespace pagoda::objects
{
class ProceduralOperation;
}

namespace pagoda::graph
{
class DefaultScheduler : public IScheduler
{
	public:
	DefaultScheduler(Graph &graph);

	void Initialize() override;
	bool Step() override;
	void Finalize() override;

	private:
	Graph &m_graph;
	std::queue<NodePtr> m_nodeQueue;
};  // class Scheduler
}  // namespace pagoda::graph
#endif
