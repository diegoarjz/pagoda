#include "graph.h"

#include "default_scheduler.h"
#include "node.h"
#include "node_factory.h"
#include "unknown_node_type.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>

#include <array>
#include <string>

namespace pagoda::graph
{
class Graph::Impl
{
private:
	using NodeWeakPtrSet = std::unordered_set<NodeWeakPtr, NodeWeakPtrHasher, NodeWeakPtrEqual>;

	struct Adjacency
	{
		NodeWeakPtrSet m_inLinks;
		NodeWeakPtrSet m_outLinks;
	};
	using AdjacencyContainer = std::unordered_map<NodeWeakPtr, Adjacency, NodeWeakPtrHasher, NodeWeakPtrEqual>;

public:
	Impl(NodeFactoryPtr nodeFactory, Graph *graph) : m_nextNodeId(0), m_graph(graph), m_nodeFactory(nodeFactory) {}

	void AddNode(NodePtr node)
	{
		node->SetId(m_nextNodeId++);
		m_nodes.insert(node);
		m_adjacencies[node] = Adjacency{};

		m_inputNodes.insert(node);
		m_outputNodes.insert(node);
	}

	void DestroyNode(NodePtr node)
	{
		NodeSet<Node> nodeInNodes = GetInNodes(node);
		NodeSet<Node> nodeOutNodes = GetOutNodes(node);

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
		m_adjacencies.erase(node);

		m_nodes.erase(node);
	}

	NodePtr GetNode(const std::string &name) const
	{
		auto iter = m_nodesByName.find(name);
		if (iter == m_nodesByName.end())
		{
			return nullptr;
		}
		return iter->second.lock();
	}

	Graph::EdgeCreated CreateEdge(NodePtr source_node, NodePtr target_node)
	{
		auto &source_node_out = OutNodes(source_node);
		auto &target_node_in = InNodes(target_node);

		auto out_link_inserted = source_node_out.insert(target_node);
		auto in_link_inserted = target_node_in.insert(source_node);

		if (!out_link_inserted.second || !in_link_inserted.second)
		{
			return EdgeCreated::EdgeExists;
		}

		m_inputNodes.erase(target_node);
		m_outputNodes.erase(source_node);

		return EdgeCreated::Created;
	}

	Graph::EdgeDestroyed DestroyEdge(NodePtr source_node, NodePtr target_node)
	{
		auto &source_node_out = OutNodes(source_node);
		auto &target_node_in = InNodes(target_node);

		size_t out_links_removed = source_node_out.erase(target_node);
		size_t in_links_removed = target_node_in.erase(source_node);

		DBG_ASSERT_MSG(out_links_removed == in_links_removed,
		               "Mismatch between number of removed in links and out links");

		if (source_node_out.size() == 0)
		{
			m_outputNodes.insert(source_node);
		}

		if (target_node_in.size() == 0)
		{
			m_inputNodes.insert(target_node);
		}

		return out_links_removed > 0 && in_links_removed > 0 ? EdgeDestroyed::Destroyed
		                                                     : EdgeDestroyed::EdgeDoesntExist;
	}

	NodeSet<Node> GetGraphNodes() { return m_nodes; }

	std::size_t GetNodeCount() const { return m_nodes.size(); }

	NodeSet<Node> GetInputNodes()
	{
		NodeSet<Node> nodes;

		for (auto n : m_inputNodes)
		{
			nodes.insert(n.lock());
		}

		return nodes;
	}

	NodeSet<Node> GetOutputNodes()
	{
		START_PROFILE;

		NodeSet<Node> nodes;

		for (auto n : m_outputNodes)
		{
			nodes.insert(n.lock());
		}

		return nodes;
	}

	NodeSet<Node> GetNodesAdjacentTo(NodePtr node)
	{
		NodeSet<Node> nodes;
		CollectNodes(InNodes(node), nodes);
		CollectNodes(OutNodes(node), nodes);
		return nodes;
	}

	NodeSet<Node> GetInNodes(NodePtr node)
	{
		NodeSet<Node> nodes;
		CollectNodes(InNodes(node), nodes);
		return nodes;
	}

	NodeSet<Node> GetOutNodes(NodePtr node)
	{
		NodeSet<Node> nodes;
		CollectNodes(OutNodes(node), nodes);
		return nodes;
	}

	void SetScheduler(std::unique_ptr<IScheduler> scheduler) { m_scheduler = std::move(scheduler); }

	void Execute()
	{
		IScheduler *scheduler = GetScheduler();
		scheduler->Initialize();
		while (true)
		{
			if (!scheduler->Step())
			{
				break;
			}
		}
		scheduler->Finalize();
	}

	NodePtr CreateNode(const std::string &nodeType) { return CreateNode(nodeType, nodeType); }

	NodePtr CreateNode(const std::string &nodeType, const std::string &nodeName)
	{
		auto node = m_nodeFactory->Create(nodeType);
		if (node == nullptr)
		{
			throw UnknownNodeTypeException(nodeType);
		}
		AddNode(node);

		auto name = nodeName;
		if (m_nodesByName.find(name) != m_nodesByName.end())
		{
			std::size_t counter = 1;
			std::string availableName = name + std::to_string(counter);
			while (m_nodesByName.find(availableName) != m_nodesByName.end())
			{
				availableName = name + std::to_string(++counter);
			}
			name = availableName;
		}

		node->SetName(name);
		m_nodesByName.emplace(name, node);

		return node;
	}

private:
	IScheduler *GetScheduler()
	{
		if (m_scheduler == nullptr)
		{
			m_scheduler = Graph::GetSchedulerFactory()(*m_graph);
		}
		return m_scheduler.get();
	}

	void CollectNodes(const NodeWeakPtrSet &node_set, NodeSet<Node> &outSet)
	{
		for (auto n : node_set)
		{
			outSet.insert(n.lock());
		}
	}

	NodeWeakPtrSet &InNodes(NodePtr node) { return m_adjacencies.find(node)->second.m_inLinks; }

	NodeWeakPtrSet &OutNodes(NodePtr node) { return m_adjacencies.find(node)->second.m_outLinks; }

	uint32_t m_nextNodeId;
	NodeSet<Node> m_nodes;
	std::unordered_map<std::string, NodeWeakPtr> m_nodesByName;
	AdjacencyContainer m_adjacencies;
	NodeWeakPtrSet m_inputNodes;
	NodeWeakPtrSet m_outputNodes;
	Graph *m_graph;
	std::unique_ptr<IScheduler> m_scheduler;
	NodeFactoryPtr m_nodeFactory;
};

Graph::Graph(NodeFactoryPtr nodeFactory) : m_implementation(std::make_unique<Graph::Impl>(nodeFactory, this)) {}

Graph::~Graph() {}

void Graph::AddNode(NodePtr node) { m_implementation->AddNode(node); }

void Graph::DestroyNode(NodePtr node) { m_implementation->DestroyNode(node); }

NodePtr Graph::GetNode(const std::string &name) const { return m_implementation->GetNode(name); }

Graph::EdgeCreated Graph::CreateEdge(NodePtr source_node, NodePtr target_node)
{
	return m_implementation->CreateEdge(source_node, target_node);
}

Graph::EdgeDestroyed Graph::DestroyEdge(NodePtr source_node, NodePtr target_node)
{
	return m_implementation->DestroyEdge(source_node, target_node);
}

NodeSet<Node> Graph::GetGraphNodes() { return m_implementation->GetGraphNodes(); }

std::size_t Graph::GetNodeCount() const { return m_implementation->GetNodeCount(); }

NodeSet<Node> Graph::GetGraphInputNodes() { return m_implementation->GetInputNodes(); }

NodeSet<Node> Graph::GetGraphOutputNodes() { return m_implementation->GetOutputNodes(); }

NodeSet<Node> Graph::GetNodesAdjacentTo(NodePtr node) { return m_implementation->GetNodesAdjacentTo(node); }

NodeSet<Node> Graph::GetNodeInputNodes(NodePtr node) { return m_implementation->GetInNodes(node); }

NodeSet<Node> Graph::GetNodeOutputNodes(NodePtr node) { return m_implementation->GetOutNodes(node); }

void Graph::SetScheduler(std::unique_ptr<IScheduler> scheduler)
{
	m_implementation->SetScheduler(std::move(scheduler));
}

NodePtr Graph::CreateNode(const std::string &nodeType) { return m_implementation->CreateNode(nodeType); }

NodePtr Graph::CreateNode(const std::string &nodeType, const std::string &nodeName)
{
	return m_implementation->CreateNode(nodeType, nodeName);
}

void Graph::Execute() { m_implementation->Execute(); }

void Graph::SetSchedulerFactory(const SchedulerFactoryFunction_t &factoryFunction)
{
	s_schedulerFactoryFunction = factoryFunction;
}

Graph::SchedulerFactoryFunction_t Graph::GetSchedulerFactory() { return s_schedulerFactoryFunction; }

Graph::SchedulerFactoryFunction_t Graph::s_schedulerFactoryFunction = [](Graph &graph) {
	return std::make_unique<DefaultScheduler>(graph);
};
}  // namespace pagoda::graph
