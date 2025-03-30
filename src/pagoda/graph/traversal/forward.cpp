#include "forward.h"
#include "pagoda/graph/node.h"

#include <algorithm>
#include <map>
#include <pagoda/graph/query/input_node.h>
#include <iterator>

#include <pagoda/graph/graph.h>

namespace pagoda::graph::traversal
{
Forward::Forward(Graph& graph) : Traversal(graph)
{
  std::queue<NodePtr> nodes;
  std::map<NodePtr, uint32_t> nodeDistances;
	query::InputNode q(graph, [&nodes, &nodeDistances](NodePtr n) {
    nodes.push(n);
    nodeDistances.emplace(n, 0);
  });
	graph.ExecuteQuery(q);

  while (!nodes.empty()) {
    NodePtr front = nodes.front();
    nodes.pop();
    const uint32_t thisNodeDistance = nodeDistances[front];

    NodeSet outNodes;
    GetOutputNodes(front, std::inserter(outNodes, std::end(outNodes)));
    for (auto n : outNodes) {
      auto iter = nodeDistances.find(n);
      const auto nextNodeDistance = iter == nodeDistances.end() ?
                                      0 :             // First time seeing this node
                                      iter->second;   // We've seen this node

      // Update distance
      if (thisNodeDistance + 1 > nextNodeDistance) {
        // We have found a longer path
        nodeDistances[n] = iter->second = thisNodeDistance + 1;
      }

      nodes.push(n);
    }
  }

  std::vector<NodePtr> sortedNodes;
  sortedNodes.reserve(nodeDistances.size());
  for (const auto& [n, dist] : nodeDistances) {
    sortedNodes.emplace_back(n);
  }

  std::sort(sortedNodes.begin(), sortedNodes.end(),
    [&nodeDistances] (const NodePtr& lhs, const NodePtr& rhs) {
      return nodeDistances[lhs] < nodeDistances[rhs];
  });

  for (const auto& n : sortedNodes) {
    m_nodesToVisit.push(n);
  }
}

Forward::~Forward() {}

NodePtr Forward::Get() { return m_nodesToVisit.front(); }

bool Forward::Advance()
{
  m_nodesToVisit.pop();
	return HasNext();
}

bool Forward::HasNext() { return m_nodesToVisit.size() > 0; }

}  // namespace pagoda::graph::traversal
