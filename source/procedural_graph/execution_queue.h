#ifndef SELECTOR_PROCEDURAL_GRAPH_EXECUTION_QUEUE_H_
#define SELECTOR_PROCEDURAL_GRAPH_EXECUTION_QUEUE_H_

#include <memory>

namespace selector
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;
class Node;
using NodePtr = std::shared_ptr<Node>;

class ExecutionQueue
{
public:
	ExecutionQueue(GraphPtr graph);
	~ExecutionQueue();

	NodePtr GetNextNode();

	void Reset();

	std::size_t GetNodeCount() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace selector

#endif
