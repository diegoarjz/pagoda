#include "graph.h"

#include "default_scheduler.h"
#include "node.h"
#include "node_factory.h"
#include "unknown_node_type.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>
#include <pagoda/common/exception/exception.h>

#include <pagoda/dynamic/dynamic_value_base.h>

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

	void DestroyNode(const NodeIdentifier_t &nodeName)
	{
		auto node = GetNode(nodeName);
		NodeSet<Node> nodeInNodes = GetInNodes(node->GetName());
		NodeSet<Node> nodeOutNodes = GetOutNodes(node->GetName());

		for (auto n : nodeInNodes)
		{
			DestroyEdge(n->GetName(), nodeName);
		}

		for (auto n : nodeOutNodes)
		{
			DestroyEdge(nodeName, n->GetName());
		}

		m_inputNodes.erase(node);
		m_outputNodes.erase(node);
		m_adjacencies.erase(node);

		m_nodes.erase(node);
	}

	NodePtr GetNode(const NodeIdentifier_t &name) const
	{
		auto iter = m_nodesByIdentifier.find(name);
		if (iter == m_nodesByIdentifier.end())
		{
            throw common::exception::Exception("Node not found in graph: " + name);
			return nullptr;
		}
		return iter->second.lock();
	}

	Graph::EdgeCreated CreateEdge(const NodeIdentifier_t &source_node, const NodeIdentifier_t &target_node)
	{
		auto sourceNodePtr = GetNode(source_node);
		auto targetNodePtr = GetNode(target_node);
		auto &source_node_out = OutNodes(sourceNodePtr);
		auto &target_node_in = InNodes(targetNodePtr);

		auto out_link_inserted = source_node_out.insert(targetNodePtr);
		auto in_link_inserted = target_node_in.insert(sourceNodePtr);

		if (!out_link_inserted.second || !in_link_inserted.second)
		{
			return EdgeCreated::EdgeExists;
		}

		m_inputNodes.erase(targetNodePtr);
		m_outputNodes.erase(sourceNodePtr);

		return EdgeCreated::Created;
	}

	Graph::EdgeDestroyed DestroyEdge(const NodeIdentifier_t &source_node, const NodeIdentifier_t &target_node)
	{
		auto sourceNodePtr = GetNode(source_node);
		auto targetNodePtr = GetNode(target_node);
		auto &source_node_out = OutNodes(sourceNodePtr);
		auto &target_node_in = InNodes(targetNodePtr);

		size_t out_links_removed = source_node_out.erase(targetNodePtr);
		size_t in_links_removed = target_node_in.erase(sourceNodePtr);

		DBG_ASSERT_MSG(out_links_removed == in_links_removed,
		               "Mismatch between number of removed in links and out links");

		if (source_node_out.size() == 0)
		{
			m_outputNodes.insert(sourceNodePtr);
		}

		if (target_node_in.size() == 0)
		{
			m_inputNodes.insert(targetNodePtr);
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

	NodeSet<Node> GetNodesAdjacentTo(const NodeIdentifier_t &nodeName)
	{
		auto node = GetNode(nodeName);
		NodeSet<Node> nodes;
		CollectNodes(InNodes(node), nodes);
		CollectNodes(OutNodes(node), nodes);
		return nodes;
	}

	NodeSet<Node> GetInNodes(const NodeIdentifier_t &nodeName)
	{
		auto node = GetNode(nodeName);
		NodeSet<Node> nodes;
		CollectNodes(InNodes(node), nodes);
		return nodes;
	}

	NodeSet<Node> GetOutNodes(const NodeIdentifier_t &nodeName)
	{
		auto node = GetNode(nodeName);
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

	NodeIdentifier_t CreateNode(const std::string &nodeType) { return CreateNode(nodeType, nodeType); }

	NodeIdentifier_t CreateNode(const std::string &nodeType, const NodeIdentifier_t &nodeName)
	{
		auto node = m_nodeFactory->Create(nodeType);
		if (node == nullptr)
		{
			throw UnknownNodeTypeException(nodeType);
		}

		node->SetId(m_nextNodeId++);
		m_nodes.insert(node);
		m_adjacencies[node] = Adjacency{};

		m_inputNodes.insert(node);
		m_outputNodes.insert(node);

		auto name = nodeName;
		if (m_nodesByIdentifier.find(name) != m_nodesByIdentifier.end())
		{
			std::size_t counter = 1;
			std::string availableName = name + std::to_string(counter);
			while (m_nodesByIdentifier.find(availableName) != m_nodesByIdentifier.end())
			{
				availableName = name + std::to_string(++counter);
			}
			name = availableName;
		}

		node->SetName(name);
		m_nodesByIdentifier.emplace(name, node);

		return name;
	}

	void SetNodeConstructionParameters(const NodeIdentifier_t &nodeName,
	                                   const std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &args)
	{
		auto node = GetNode(nodeName);
		if (node == nullptr)
		{
			return;
		}
		node->SetConstructionArguments(args);
	}

	void SetNodeExecutionParameters(const NodeIdentifier_t &nodeName,
	                                const std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &args)
	{
		auto node = GetNode(nodeName);
		if (node == nullptr)
		{
			return;
		}
		node->SetExecutionArguments(args);
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
	std::unordered_map<NodeIdentifier_t, NodeWeakPtr> m_nodesByIdentifier;
	AdjacencyContainer m_adjacencies;
	NodeWeakPtrSet m_inputNodes;
	NodeWeakPtrSet m_outputNodes;
	Graph *m_graph;
	std::unique_ptr<IScheduler> m_scheduler;
	NodeFactoryPtr m_nodeFactory;
};

Graph::Graph(NodeFactoryPtr nodeFactory) : m_implementation(std::make_unique<Graph::Impl>(nodeFactory, this)) {}

Graph::~Graph() {}

void Graph::DestroyNode(const NodeIdentifier_t &node) { m_implementation->DestroyNode(node); }

NodePtr Graph::GetNode(const NodeIdentifier_t &name) const { return m_implementation->GetNode(name); }

Graph::EdgeCreated Graph::CreateEdge(const NodeIdentifier_t &source_node, const NodeIdentifier_t &target_node)
{
	return m_implementation->CreateEdge(source_node, target_node);
}

Graph::EdgeDestroyed Graph::DestroyEdge(const NodeIdentifier_t &source_node, const NodeIdentifier_t &target_node)
{
	return m_implementation->DestroyEdge(source_node, target_node);
}

NodeSet<Node> Graph::GetGraphNodes() { return m_implementation->GetGraphNodes(); }

std::size_t Graph::GetNodeCount() const { return m_implementation->GetNodeCount(); }

NodeSet<Node> Graph::GetGraphInputNodes() { return m_implementation->GetInputNodes(); }

NodeSet<Node> Graph::GetGraphOutputNodes() { return m_implementation->GetOutputNodes(); }

NodeSet<Node> Graph::GetNodesAdjacentTo(const NodeIdentifier_t &node)
{
	return m_implementation->GetNodesAdjacentTo(node);
}

NodeSet<Node> Graph::GetNodeInputNodes(const NodeIdentifier_t &node) { return m_implementation->GetInNodes(node); }

NodeSet<Node> Graph::GetNodeOutputNodes(const NodeIdentifier_t &node) { return m_implementation->GetOutNodes(node); }

void Graph::SetScheduler(std::unique_ptr<IScheduler> scheduler)
{
	m_implementation->SetScheduler(std::move(scheduler));
}

Graph::NodeIdentifier_t Graph::CreateNode(const std::string &nodeType)
{
	return m_implementation->CreateNode(nodeType);
}

Graph::NodeIdentifier_t Graph::CreateNode(const std::string &nodeType, const NodeIdentifier_t &nodeName)
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

void Graph::SetNodeConstructionParameters(const NodeIdentifier_t &nodeName,
                                          const std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &args)
{
	m_implementation->SetNodeConstructionParameters(nodeName, args);
}

void Graph::SetNodeExecutionParameters(const NodeIdentifier_t &nodeName,
                                       const std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &args)
{
	m_implementation->SetNodeExecutionParameters(nodeName, args);
}
}  // namespace pagoda::graph
