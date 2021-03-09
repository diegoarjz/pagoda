#include "graph_scene.h"

#include "graph_connector.h"
#include "graph_in_port.h"
#include "graph_node.h"
#include "graph_out_port.h"
#include "node_style.h"

#include <pagoda/graph/query/topology.h>
#include <pagoda/graph/query/type.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node_set.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/traversal/forward.h>

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/value_not_found.h>

#include <iostream>
#include <unordered_map>

using namespace pagoda::graph;
using namespace pagoda::graph::query;
using namespace pagoda::graph::traversal;

using namespace pagoda::dynamic;

namespace pgeditor::gui
{
GraphScene::GraphScene() {}

void GraphScene::SetProceduralGraph(std::shared_ptr<pagoda::graph::Graph> graph)
{
	m_graph = graph;

	NodeSet operationNodes;
	auto opNodesQuery = type<OperationNode>(*m_graph, operationNodes);
	m_graph->ExecuteQuery(opNodesQuery);

	bool needsLayout = false;
	for (const auto& n : operationNodes) {
		GraphNode* node = new GraphNode(n, m_graph);
		this->addItem(node);
		m_operationNodes[n.get()] = node;

		try {
			auto posX = n->GetMember("posX");
			auto posY = n->GetMember("posY");

			if (posX != nullptr && posY != nullptr) {
				node->setPos({static_cast<qreal>(get_value_as<int>(*posX)), static_cast<qreal>(get_value_as<int>(*posY))});
			} else {
				needsLayout = true;
			}
		} catch (ValueNotFoundException& e) {
			needsLayout = true;
		}
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

	if (needsLayout) {
		LayoutGraph();
	}
}

void GraphScene::LayoutGraph()
{
	if (m_graph == nullptr) {
		return;
	}

	Forward f(*m_graph);
	using namespace node_style::autolayout;

	std::unordered_map<Node*, int32_t> nodeDepths;
	int32_t maxNodeDepth = 0;
	while (f.HasNext()) {
		auto n = f.Get();

		auto [iter, inserted] = nodeDepths.try_emplace(n.get(), 0);
		int32_t maxDepth = -1;
		for (const auto& in : m_graph->GetNodeInputNodes(n->GetName())) {
			maxDepth = std::max(maxDepth, nodeDepths[in.get()]);
		}
		iter->second = maxDepth + 1;
		maxNodeDepth = std::max(maxNodeDepth, iter->second);

		f.Advance();
	}

	std::vector<int32_t> verticalPosition(maxNodeDepth + 1, 0);
	for (auto& n : m_operationNodes) {
		auto node = n.first;
		auto guiNode = n.second;
		auto depth = nodeDepths[node];
		guiNode->setPos(
		  {static_cast<qreal>(depth * horizontal_spacing), static_cast<qreal>(verticalPosition[depth] * vertical_spacing)});
		verticalPosition[depth]++;
	}
}
}  // namespace pgeditor::gui
