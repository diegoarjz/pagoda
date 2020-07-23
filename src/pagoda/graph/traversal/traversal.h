#pragma once

#include <pagoda/graph/node_set.h>

#include <iterator>

namespace pagoda::graph
{
class Graph;
}

namespace pagoda::graph::traversal
{
class Traversal
{
public:
	virtual NodePtr Get() = 0;
	virtual bool Advance() = 0;
	virtual bool HasNext() = 0;

	void ForEachUntil(std::function<bool(NodePtr)> handle);
	void ForEach(std::function<void(NodePtr)> handle);

protected:
	Traversal(Graph& graph);

	void GetInputNodes(const NodePtr& node, std::insert_iterator<NodeSet<Node>> inserter);
	void GetOutputNodes(const NodePtr& node, std::insert_iterator<NodeSet<Node>> inserter);
	void GetAdjacentNodes(const NodePtr& node, std::insert_iterator<NodeSet<Node>> inserter);
	NodeSet<Node>& GetNodes() const;

private:
	Graph& m_graph;
};
}  // namespace pagoda::graph::traversal
