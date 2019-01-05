#include "graph.h"

#include "common/assertions.h"
#include "node.h"
#include "node_factory.h"

#include <array>

namespace selector
{
class Graph::Impl
{
public:
	Impl(std::shared_ptr<NodeFactory> nodeFactory) : m_nodeFactory(nodeFactory) {}

	NodePtr CreateNode(NodeType type, const std::string &name = "")
	{
		NodePtr node = m_nodeFactory->Create(type);
		m_nodesByType[Index(type)].insert(node);
		node->SetId(m_nextNodeId++);
		m_adjacencies.emplace_back();

		m_inputNodes.insert(node);
		m_outputNodes.insert(node);

		return node;
	}

	void DestroyNode(NodePtr node)
	{
		std::list<NodePtr> nodeInNodes = GetInNodes(node);
		std::list<NodePtr> nodeOutNodes = GetOutNodes(node);

		for (auto n : nodeInNodes)
		{
			DestroyEdge(n, node);
		}

		for (auto n : nodeOutNodes)
		{
			DestroyEdge(node, n);
		}

		m_inputNodes.erase(node);
		m_outputNodes.erase(node);

		m_nodesByType[Index(node->GetNodeType())].erase(node);
	}

	const std::unordered_set<NodePtr> &GetNodesByType(NodeType node_type) { return m_nodesByType[Index(node_type)]; }

	Graph::EdgeCreated CreateEdge(NodePtr source_node, NodePtr target_node)
	{
		// TODO: validate that nodes can be connected
		auto &source_node_out = OutNodes(source_node)[Index(target_node->GetNodeType())];
		auto &target_node_in = InNodes(target_node)[Index(source_node->GetNodeType())];

		auto out_link_inserted = source_node_out.insert(target_node);
		auto in_link_inserted = target_node_in.insert(source_node);

		if (!out_link_inserted.second || !in_link_inserted.second)
		{
			source_node_out.erase(out_link_inserted.first);
			target_node_in.erase(in_link_inserted.first);
			return EdgeCreated::EdgeExists;
		}

		m_inputNodes.erase(target_node);
		m_outputNodes.erase(source_node);

		return EdgeCreated::Created;
	}

	Graph::EdgeDestroyed DestroyEdge(NodePtr source_node, NodePtr target_node)
	{
		auto &source_node_out = OutNodes(source_node)[Index(target_node->GetNodeType())];
		auto &target_node_in = InNodes(target_node)[Index(source_node->GetNodeType())];

		size_t out_links_removed = source_node_out.erase(target_node);
		size_t in_links_removed = target_node_in.erase(source_node);

		DBG_ASSERT_MSG(out_links_removed == in_links_removed,
		               "Mismatch between number of removed in links and out links");

		if (CountAdjacencies(OutNodes(source_node)) == 0)
		{
			m_outputNodes.insert(source_node);
		}

		if (CountAdjacencies(InNodes(target_node)) == 0)
		{
			m_inputNodes.insert(target_node);
		}

		return out_links_removed > 0 && in_links_removed > 0 ? EdgeDestroyed::Destroyed
		                                                     : EdgeDestroyed::EdgeDoesntExist;
	}

	std::list<NodePtr> GetInputNodes()
	{
		std::list<NodePtr> nodes;

		for (auto n : m_inputNodes)
		{
			nodes.push_back(n.lock());
		}

		return nodes;
	}

	std::list<NodePtr> GetOutputNodes()
	{
		START_PROFILE;

		std::list<NodePtr> nodes;

		for (auto n : m_outputNodes)
		{
			nodes.push_back(n.lock());
		}

		return nodes;
	}

	std::list<NodePtr> GetAdjacentNodes(NodePtr node)
	{
		std::list<NodePtr> nodes;
		auto inserter = std::back_inserter(nodes);
		CollectNodes(InNodes(node), inserter);
		CollectNodes(OutNodes(node), inserter);
		return nodes;
	}

	std::list<NodePtr> GetAdjacentNodesByType(NodePtr node, NodeType type)
	{
		std::list<NodePtr> nodes;
		auto inserter = std::back_inserter(nodes);
		CollectNodes(InNodes(node)[Index(type)], inserter);
		CollectNodes(OutNodes(node)[Index(type)], inserter);
		return nodes;
	}

	std::list<NodePtr> GetInNodes(NodePtr node)
	{
		std::list<NodePtr> nodes;
		CollectNodes(InNodes(node), std::back_inserter(nodes));
		return nodes;
	}

	std::list<NodePtr> GetInNodesByType(NodePtr node, NodeType type)
	{
		std::list<NodePtr> nodes;
		CollectNodes(InNodes(node)[Index(type)], std::back_inserter(nodes));
		return nodes;
	}

	std::list<NodePtr> GetOutNodes(NodePtr node)
	{
		std::list<NodePtr> nodes;
		CollectNodes(OutNodes(node), std::back_inserter(nodes));
		return nodes;
	}

	std::list<NodePtr> GetOutNodesByType(NodePtr node, NodeType type)
	{
		std::list<NodePtr> nodes;
		CollectNodes(OutNodes(node)[Index(type)], std::back_inserter(nodes));
		return nodes;
	}

private:
	using NodePtrSet = std::unordered_set<NodePtr>;
	using NodeContainer = std::array<NodePtrSet, Index(NodeType::MAX)>;
	using NodeWeakPtrSet = std::unordered_set<NodeWeakPtr, NodeWeakPtrHasher, NodeWeakPtrEqual>;
	using NodeAdjacencyContainer = std::array<NodeWeakPtrSet, Index(NodeType::MAX)>;

	struct Adjacency
	{
		NodeAdjacencyContainer m_inLinks;
		NodeAdjacencyContainer m_outLinks;
	};

	template<class C>
	void CollectNodes(const NodeWeakPtrSet &node_set, std::back_insert_iterator<C> inserter)
	{
		for (auto n : node_set)
		{
			inserter = n.lock();
		}
	}

	template<class C>
	void CollectNodes(const NodeAdjacencyContainer &adjacency, std::back_insert_iterator<C> inserter)
	{
		for (auto node_type_adjacencies : adjacency)
		{
			CollectNodes(node_type_adjacencies, inserter);
		}
	}

	Adjacency &AdjacencyForNode(NodePtr node) { return m_adjacencies[node->GetId()]; }
	NodeAdjacencyContainer &InNodes(NodePtr node) { return AdjacencyForNode(node).m_inLinks; }
	NodeAdjacencyContainer &OutNodes(NodePtr node) { return AdjacencyForNode(node).m_outLinks; }

	size_t CountAdjacencies(const NodeAdjacencyContainer &adjacencies) const
	{
		size_t count = 0;

		for (const auto &adj : adjacencies)
		{
			count += adj.size();
		}

		return count;
	}

	uint32_t m_nextNodeId;

	std::shared_ptr<NodeFactory> m_nodeFactory;
	std::vector<Adjacency> m_adjacencies;
	NodeContainer m_nodesByType;
	NodeWeakPtrSet m_inputNodes;
	NodeWeakPtrSet m_outputNodes;
};

Graph::Graph(std::shared_ptr<NodeFactory> nodeFactory) : m_implementation(std::make_unique<Graph::Impl>(nodeFactory)) {}

Graph::~Graph() {}

NodePtr Graph::CreateNode(NodeType type, const std::string &name) { return m_implementation->CreateNode(type, name); }

void Graph::DestroyNode(NodePtr node) { m_implementation->DestroyNode(node); }

const std::unordered_set<NodePtr> &Graph::GetGraphNodesByType(NodeType node_type)
{
	return m_implementation->GetNodesByType(node_type);
}

Graph::EdgeCreated Graph::CreateEdge(NodePtr source_node, NodePtr target_node)
{
	return m_implementation->CreateEdge(source_node, target_node);
}

Graph::EdgeDestroyed Graph::DestroyEdge(NodePtr source_node, NodePtr target_node)
{
	return m_implementation->DestroyEdge(source_node, target_node);
}

std::list<NodePtr> Graph::GetGraphInputNodes() { return m_implementation->GetInputNodes(); }

std::list<NodePtr> Graph::GetGraphOutputNodes() { return m_implementation->GetOutputNodes(); }

std::list<NodePtr> Graph::GetNodesLinkedTo(NodePtr node) { return m_implementation->GetAdjacentNodes(node); }

std::list<NodePtr> Graph::GetNodesLinkedToByType(NodePtr node, NodeType type)
{
	return m_implementation->GetAdjacentNodesByType(node, type);
}

std::list<NodePtr> Graph::GetNodeInputNodes(NodePtr node) { return m_implementation->GetInNodes(node); }

std::list<NodePtr> Graph::GetNodeInputNodesByType(NodePtr node, NodeType type)
{
	return m_implementation->GetInNodesByType(node, type);
}

std::list<NodePtr> Graph::GetNodeOutputNodes(NodePtr node) { return m_implementation->GetOutNodes(node); }

std::list<NodePtr> Graph::GetNodeOutputNodesByType(NodePtr node, NodeType type)
{
	return m_implementation->GetOutNodesByType(node, type);
}

}  // namespace selector
