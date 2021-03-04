#include "graph_scene.h"

#include "graph_connector.h"
#include "graph_in_port.h"
#include "graph_node.h"
#include "graph_out_port.h"

#include <pagoda/graph/query/topology.h>
#include <pagoda/graph/query/type.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node_set.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>

#include <iostream>

using namespace pagoda::graph;
using namespace pagoda::graph::query;

namespace pgeditor::gui
{
GraphScene::GraphScene() {}

void GraphScene::SetProceduralGraph(std::shared_ptr<pagoda::graph::Graph> graph)
{
	m_graph = graph;

	NodeSet operationNodes;
	auto opNodesQuery = type<OperationNode>(*m_graph, operationNodes);
	m_graph->ExecuteQuery(opNodesQuery);

	for (const auto& n : operationNodes) {
		GraphNode* node = new GraphNode(n, m_graph);
		this->addItem(node);
		m_operationNodes[n.get()] = node;
	}

	auto upstreamNodeQuery = type<OperationNode>();
	auto downStreamNodeQuery = type<OperationNode>();
	auto outInterfaceQuery = type<OutputInterfaceNode>();
	auto inInterfaceQuery = type<InputInterfaceNode>();

	QueryTopology connectionQuery(&outInterfaceQuery);
	connectionQuery.AddUpstreamQuery(&outInterfaceQuery, &upstreamNodeQuery);
	connectionQuery.AddDownstreamQuery(&outInterfaceQuery, &inInterfaceQuery);
	connectionQuery.AddDownstreamQuery(&inInterfaceQuery, &downStreamNodeQuery);

	std::vector<typename QueryTopology::Match_t> matches;
	connectionQuery.Match(m_graph.get(), matches);

	for (auto& m : matches) {
		auto upstreamNode = m_operationNodes[m[&upstreamNodeQuery].get()];
		auto downStreamNode = m_operationNodes[m[&downStreamNodeQuery].get()];
		auto inInterfaceNode = m[&inInterfaceQuery];
		auto outInterfaceNode = m[&outInterfaceQuery];

		auto outPort = upstreamNode->GetOutPort(outInterfaceNode);
		auto inPort = downStreamNode->GetInPort(inInterfaceNode);

		GraphConnector* connector = new GraphConnector(outPort->GetPortConnection(), inPort->GetPortConnection());
		this->addItem(connector);
	}
}
}  // namespace pgeditor::gui
