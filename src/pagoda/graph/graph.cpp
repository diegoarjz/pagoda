#include "pagoda/graph/graph.h"

#include "pagoda/graph/query/input_node.h"
#include "pagoda/graph/query/output_node.h"
#include "pagoda/graph/query/query.h"

#include "pagoda/graph/traversal/linear.h"

#include "pagoda/graph/default_scheduler.h"
#include "pagoda/graph/node.h"
#include "pagoda/graph/node_factory.h"
#include "pagoda/graph/unknown_node_type.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/exception/exception.h>
#include <pagoda/common/instrument/profiler.h>
#include <algorithm>

#include <pagoda/dynamic/dynamic_value_base.h>

#include <array>
#include <string>

namespace pagoda::graph
{
class Graph::Impl
{
	private:
	using NodeWeakPtrSet =
	  std::unordered_set<NodeWeakPtr, NodeWeakPtrHasher, NodeWeakPtrEqual>;

	struct Adjacency
	{
		NodeWeakPtrSet m_inLinks;
		NodeWeakPtrSet m_outLinks;
	};
	using AdjacencyContainer = std::unordered_map<NodeIdentifier_t, Adjacency>;

	public:
	Impl(NodeFactoryPtr nodeFactory, Graph *graph)
	  : m_nextNodeId(0), m_graph(graph), m_nodeFactory(nodeFactory)
	{
	}

	void DestroyNode(const NodeIdentifier_t &nodeName)
	{
		auto node = GetNode(nodeName);
		DBG_ASSERT_MSG(node != nullptr, "Node doesn't exist in graph");
		std::size_t nodeCount = m_nodes.size();

		NodeSet nodeInNodes = GetInNodes(nodeName);
		NodeSet nodeOutNodes = GetOutNodes(nodeName);

		for (auto n : nodeInNodes) {
			DestroyEdge(n->GetName(), nodeName);
		}

		for (auto n : nodeOutNodes) {
			DestroyEdge(nodeName, n->GetName());
		}

		m_adjacencies.erase(nodeName);
		m_nodes.erase(node);
		m_nodesByIdentifier.erase(nodeName);
		DBG_ASSERT(m_nodes.size() == nodeCount - 1);
		DBG_ASSERT(GetNode(nodeName) == nullptr);
	}

	NodePtr GetNode(const NodeIdentifier_t &name) const
	{
		auto iter = m_nodesByIdentifier.find(name);
		if (iter == m_nodesByIdentifier.end()) {
			return nullptr;
		}
		return iter->second.lock();
	}

  bool RenameNode(const NodeIdentifier_t &name, const NodeIdentifier_t& newName) {
    if (GetNode(newName) != nullptr && name != newName) {
      return false;
    }

    if (auto node = GetNode(name)) {
      m_nodesByIdentifier.erase(name);
      m_nodesByIdentifier.emplace(newName, node);
      
      auto adjacencyIter = m_adjacencies.find(name);
      m_adjacencies.emplace(newName, std::move(adjacencyIter->second));
      m_adjacencies.erase(name);

      node->SetName(newName);

      auto in = InNodes(newName);
      auto out = OutNodes(newName);
    }
    else {
      return false;
    }

    return true;
  }

	Graph::EdgeCreated CreateEdge(const NodeIdentifier_t &source_node,
	                              const NodeIdentifier_t &target_node)
	{
		auto &source_node_out = OutNodes(source_node);
		auto &target_node_in = InNodes(target_node);

		auto out_link_inserted = source_node_out.insert(GetNode(target_node));
		auto in_link_inserted = target_node_in.insert(GetNode(source_node));

		if (!out_link_inserted.second || !in_link_inserted.second) {
			return EdgeCreated::EdgeExists;
		}

		return EdgeCreated::Created;
	}

	Graph::EdgeDestroyed DestroyEdge(const NodeIdentifier_t &source_node,
	                                 const NodeIdentifier_t &target_node)
	{
		auto &source_node_out = OutNodes(source_node);
		auto &target_node_in = InNodes(target_node);

		size_t out_links_removed = source_node_out.erase(GetNode(target_node));
		size_t in_links_removed = target_node_in.erase(GetNode(source_node));

		DBG_ASSERT_MSG(out_links_removed == in_links_removed,
		               "Mismatch between number of removed in links and out links");

		return out_links_removed > 0 && in_links_removed > 0
		         ? EdgeDestroyed::Destroyed
		         : EdgeDestroyed::EdgeDoesntExist;
	}

	std::size_t GetNodeCount() const
	{
		return m_nodes.size();
	}

	NodeSet GetNodesAdjacentTo(const NodeIdentifier_t &nodeName)
	{
		NodeSet nodes;
		CollectNodes(InNodes(nodeName), nodes);
		CollectNodes(OutNodes(nodeName), nodes);
		return nodes;
	}

	NodeSet GetInNodes(const NodeIdentifier_t &nodeName)
	{
		NodeSet nodes;
		CollectNodes(InNodes(nodeName), nodes);
		return nodes;
	}

	NodeSet GetOutNodes(const NodeIdentifier_t &nodeName)
	{
		NodeSet nodes;
		CollectNodes(OutNodes(nodeName), nodes);
		return nodes;
	}

	void SetScheduler(std::unique_ptr<IScheduler> scheduler)
	{
		m_scheduler = std::move(scheduler);
	}

	void Execute()
	{
		IScheduler *scheduler = GetScheduler();
		scheduler->Initialize();
		while (true) {
			if (!scheduler->Step()) {
				break;
			}
		}
		scheduler->Finalize();
	}

	NodeIdentifier_t CreateNode(const std::string &nodeType)
	{
		return CreateNode(nodeType, nodeType);
	}

	NodeIdentifier_t CreateNode(const std::string &nodeType,
	                            const NodeIdentifier_t &nodeName)
	{
		auto node = m_nodeFactory->Create(nodeType);
		if (node == nullptr) {
			throw UnknownNodeTypeException(nodeType);
		}

		auto name = nodeName;
		if (m_nodesByIdentifier.find(name) != m_nodesByIdentifier.end()) {
			std::size_t counter = 1;
			std::string availableName = name + std::to_string(counter);
			while (m_nodesByIdentifier.find(availableName) !=
			       m_nodesByIdentifier.end()) {
				availableName = name + std::to_string(++counter);
			}
			name = availableName;
		}

		node->SetName(name);
		m_nodesByIdentifier.emplace(name, node);
		node->SetId(m_nextNodeId++);
		m_nodes.insert(node);
		m_adjacencies[name] = Adjacency{};

    node->AttachToGraph(m_graph);

		return name;
	}

	void ExecuteQuery(query::Query &q)
	{
		START_PROFILE;
		DBG_ASSERT(q.GetGraph() != nullptr);

		auto t = q.GetTraversal();
		DBG_ASSERT(t != nullptr);
		while (t->HasNext()) {
			NodePtr n = t->Get();
			if (q.Matches(n)) {
				q.AddNode(n);
			}
			t->Advance();
		}
	}

	NodeSet &getNodes()
	{
		return m_nodes;
	}

	private:
	IScheduler *GetScheduler()
	{
		if (m_scheduler == nullptr) {
			m_scheduler = Graph::GetSchedulerFactory()(*m_graph);
		}
		return m_scheduler.get();
	}

	void CollectNodes(const NodeWeakPtrSet &node_set, NodeSet &outSet)
	{
		std::transform(node_set.begin(), node_set.end(),
		               std::inserter(outSet, outSet.end()),
		               [](NodeWeakPtr n) { return n.lock(); });
	}

	NodeWeakPtrSet &InNodes(NodeIdentifier_t node)
	{
		auto adjacenies = m_adjacencies.find(node);
		if (adjacenies == m_adjacencies.end()) {
			throw common::exception::Exception("Node '" + node +
			                                   "' not found in graph");
		}

		return adjacenies->second.m_inLinks;
	}

	NodeWeakPtrSet &OutNodes(NodeIdentifier_t node)
	{
		auto adjacenies = m_adjacencies.find(node);
		if (adjacenies == m_adjacencies.end()) {
			throw common::exception::Exception("Node '" + node +
			                                   "' not found in graph");
		}

		return adjacenies->second.m_outLinks;
	}

	uint32_t m_nextNodeId;
	NodeSet m_nodes;
	std::unordered_map<NodeIdentifier_t, NodeWeakPtr> m_nodesByIdentifier;
	AdjacencyContainer m_adjacencies;
	Graph *m_graph;
	std::unique_ptr<IScheduler> m_scheduler;
	NodeFactoryPtr m_nodeFactory;
};

Graph::Graph(NodeFactoryPtr nodeFactory)
  : m_implementation(std::make_unique<Graph::Impl>(nodeFactory, this))
{
}

Graph::~Graph()
{
}

void Graph::DestroyNode(const NodeIdentifier_t &node)
{
	m_implementation->DestroyNode(node);
}

NodePtr Graph::GetNode(const NodeIdentifier_t &name) const
{
	return m_implementation->GetNode(name);
}

bool Graph::RenameNode(const NodeIdentifier_t &name, const NodeIdentifier_t& newName) { return m_implementation->RenameNode(name, newName); }

Graph::EdgeCreated Graph::CreateEdge(const NodeIdentifier_t &source_node,
                                     const NodeIdentifier_t &target_node)
{
	return m_implementation->CreateEdge(source_node, target_node);
}

Graph::EdgeDestroyed Graph::DestroyEdge(const NodeIdentifier_t &source_node,
                                        const NodeIdentifier_t &target_node)
{
	return m_implementation->DestroyEdge(source_node, target_node);
}

std::size_t Graph::GetNodeCount() const
{
	return m_implementation->GetNodeCount();
}

NodeSet Graph::GetNodesAdjacentTo(const NodeIdentifier_t &node)
{
	return m_implementation->GetNodesAdjacentTo(node);
}

NodeSet Graph::GetNodeInputNodes(const NodeIdentifier_t &node)
{
	return m_implementation->GetInNodes(node);
}

NodeSet Graph::GetNodeOutputNodes(const NodeIdentifier_t &node)
{
	return m_implementation->GetOutNodes(node);
}

void Graph::SetScheduler(std::unique_ptr<IScheduler> scheduler)
{
	m_implementation->SetScheduler(std::move(scheduler));
}

Graph::NodeIdentifier_t Graph::CreateNode(const std::string &nodeType)
{
	return m_implementation->CreateNode(nodeType);
}

Graph::NodeIdentifier_t Graph::CreateNode(const std::string &nodeType,
                                          const NodeIdentifier_t &nodeName)
{
	return m_implementation->CreateNode(nodeType, nodeName);
}

void Graph::Execute()
{
	m_implementation->Execute();
}

void Graph::SetSchedulerFactory(
  const SchedulerFactoryFunction_t &factoryFunction)
{
	s_schedulerFactoryFunction = factoryFunction;
}

Graph::SchedulerFactoryFunction_t Graph::GetSchedulerFactory()
{
	return s_schedulerFactoryFunction;
}

Graph::SchedulerFactoryFunction_t Graph::s_schedulerFactoryFunction =
  [](Graph &graph) { return std::make_unique<DefaultScheduler>(graph); };

void Graph::ExecuteQuery(query::Query &q)
{
	m_implementation->ExecuteQuery(q);
}

NodeSet &Graph::getNodes()
{
	return m_implementation->getNodes();
}
}  // namespace pagoda::graph
