#ifndef SELECTOR_PROCEDURAL_GRAPH_EXECUTION_QUEUE_H_
#define SELECTOR_PROCEDURAL_GRAPH_EXECUTION_QUEUE_H_

#include <memory>

namespace selector
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;
class Node;
using NodePtr = std::shared_ptr<Node>;

/**
 * Implements a queue of \c Node that reflect the order in which they will be executed.
 */
class ExecutionQueue
{
public:
    /**
     * Constructs the \c ExecutionQueue from a \c Graph.
     */
	ExecutionQueue(Graph& graph);
	~ExecutionQueue();

    /**
     * Returns the next \c Node in the queue.
     * @return the next \c Node or nullptr if empty.
     */
	NodePtr GetNextNode();

    /**
     * Resets the \c ExecutionQueue to the beginning of the queue.
     * Nodes are not deleted and the order is kept.
     */
	void Reset();

    /**
     * Returns the number of \c Node in the queue.
     */
	std::size_t GetNodeCount() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace selector

#endif
