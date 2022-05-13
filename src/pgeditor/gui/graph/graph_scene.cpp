#include "graph_scene.h"

#include "graph_connector.h"
#include "graph_in_port.h"
#include "graph_node.h"
#include "graph_out_port.h"
#include "graph_port_connection.h"
#include "new_connection_edge.h"
#include "new_node_popup.h"
#include "node_style.h"
#include "operation_node.h"
#include "pagoda/objects/interface_callback.h"
#include "pagoda/objects/operation_factory.h"

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

#include <pagoda/pagoda.h>

#include <QCursor>
#include <QGraphicsSceneDragDropEvent>

#include <unordered_map>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;
using namespace pagoda::graph::traversal;

using namespace pagoda::dynamic;

namespace pgeditor::gui::graph
{
GraphScene::GraphScene(Pagoda* pagoda) : m_pagoda{pagoda} {}

void GraphScene::SetProceduralGraph(std::shared_ptr<pagoda::graph::Graph> graph)
{
	m_graph = graph;

	NodeSet operationNodes;
	auto opNodesQuery = type<pagoda::graph::OperationNode>(*m_graph, operationNodes);
	m_graph->ExecuteQuery(opNodesQuery);

	bool needsLayout = false;
	for (const auto& n : operationNodes) {
		OperationNode* node = new OperationNode(m_graph, n);
		node->InitializeGUI();
		this->addItem(node);
		m_operationNodes[n.get()] = node;

		try {
			auto posX = n->GetMember("posX");
			auto posY = n->GetMember("posY");

			if (posX != nullptr && posY != nullptr) {
				node->SetPosition(get_value_as<int>(*posX), get_value_as<int>(*posY));
			} else {
				needsLayout = true;
			}
		} catch (ValueNotFoundException& e) {
			needsLayout = true;
		}

		node->ForEachPort([this](GraphPort* port) {
			connect(port, &GraphPort::StartConnectionDrag, this, &GraphScene::StartConnectionDrag);
		});
		connect(node, &GraphNode::NewConnection, this, &GraphScene::ConnectedNodes);
	}

	auto upstreamNodeQuery = type<pagoda::graph::OperationNode>();
	auto downStreamNodeQuery = type<pagoda::graph::OperationNode>();
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

		auto outPort = dynamic_cast<OperationNode*>(upstreamNode)->GetOutPortForNode(outInterfaceNode);
		auto inPort = dynamic_cast<OperationNode*>(downStreamNode)->GetInPortForNode(inInterfaceNode);

		ConnectedNodes(outPort, inPort);
	}

	if (needsLayout) {
		LayoutGraph();
	}
}

void GraphScene::StartConnectionDrag(GraphPort* port, NewConnectionEdge* newEdge) { m_newEdge = newEdge; }

void GraphScene::ConnectedNodes(GraphPort* from, GraphPort* to)
{
	GraphConnector* connector = new GraphConnector(from->GetPortConnection(), to->GetPortConnection());
	this->addItem(connector);
	connector->PortConnectionChanged(nullptr, {});
	m_connectorsByNode[from->GetPortConnection()->GetNode()].insert(connector);
	m_connectorsByNode[to->GetPortConnection()->GetNode()].insert(connector);
}

namespace {
using namespace pagoda::objects;

class Creator : public pagoda::objects::InterfaceCallback {
public:
  Creator(const std::shared_ptr<Graph>& graph, const std::string& nodeName)
    : m_graph{graph}, m_nodeName{nodeName}
  {}

  void InputInterface(InterfacePtr& interface, const std::string& name, const std::string& label, Interface::Arity arity) override
  {
		const std::string inName = m_graph->CreateNode<InputInterfaceNode>(m_nodeName + "_" + name);
		auto inNode = std::dynamic_pointer_cast<InputInterfaceNode>(m_graph->GetNode(inName));
		inNode->SetInterfaceName(name);
		m_graph->CreateEdge(inName, m_nodeName);
  }

  void OutputInterface(InterfacePtr& interface, const std::string& name, const std::string& label, Interface::Arity arity) override
  {
		const std::string outName = m_graph->CreateNode<OutputInterfaceNode>(m_nodeName + "_" + name);
		auto outNode = std::dynamic_pointer_cast<OutputInterfaceNode>(m_graph->GetNode(outName));
		outNode->SetInterfaceName(name);
		m_graph->CreateEdge(m_nodeName, outName);
  }

  private:
  const std::shared_ptr<Graph>& m_graph;
  const std::string& m_nodeName;
};
}

GraphNode* GraphScene::createOperation(const QString& opName)
{
	auto operation = m_pagoda->GetOperationFactory()->Create(opName.toStdString());
	if (operation == nullptr) {
    LOG_ERROR("No operation named '" << opName.toStdString() << "' found.");
    return nullptr;
	}

	auto nodeName = m_graph->CreateNode<pagoda::graph::OperationNode>(opName.toStdString());
	auto node = m_graph->GetNode(nodeName);
	std::dynamic_pointer_cast<pagoda::graph::OperationNode>(node)->SetOperation(operation);

  Creator creator{m_graph, nodeName};
  operation->Interfaces(&creator);

	OperationNode* guiNode = new OperationNode(m_graph, node);
	guiNode->InitializeGUI();
	m_operationNodes[node.get()] = guiNode;
	guiNode->ForEachPort([this](GraphPort* port) {
		connect(port, &GraphPort::StartConnectionDrag, this, &GraphScene::StartConnectionDrag);
	});
	connect(guiNode, &GraphNode::NewConnection, this, &GraphScene::ConnectedNodes);

	this->addItem(guiNode);
	return guiNode;
}

void GraphScene::deleteOperationNode(GraphNode* operationNode)
{
	auto n = operationNode->GetNode();
	auto inNodes = m_graph->GetNodeInputNodes(n->GetName());
	auto outNodes = m_graph->GetNodeOutputNodes(n->GetName());

	for (auto i : inNodes) {
		m_graph->DestroyNode(i->GetName());
	}
	for (auto o : outNodes) {
		m_graph->DestroyNode(o->GetName());
	}
	m_graph->DestroyNode(n->GetName());
	m_operationNodes.erase(n.get());
	removeItem(operationNode);

	for (const auto& connection : m_connectorsByNode[operationNode]) {
		removeItem(connection);
		auto otherNode = connection->GetDownstreamNode() == operationNode ? connection->GetUpstreamNode()
		                                                                  : connection->GetDownstreamNode();
		m_connectorsByNode[otherNode].erase(connection);
		delete connection;
	}
	m_connectorsByNode.erase(operationNode);
	delete operationNode;
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
		guiNode->SetPosition(depth * horizontal_spacing, verticalPosition[depth] * vertical_spacing);
		verticalPosition[depth]++;
	}
}

void GraphScene::keyPressEvent(QKeyEvent* keyEvent)
{
	switch (keyEvent->key()) {
		case Qt::Key_Tab: {
			// Show new node pop up
			auto nodePopup = new NewNodePopup(m_pagoda->GetOperationFactory());
			auto mousePosition = QCursor::pos();
			nodePopup->move(mousePosition.x(), mousePosition.y());

			connect(nodePopup, &NewNodePopup::OperationSelected, [this /*, &mousePosition*/](const QString& op) {
				auto node = createOperation(op);
				if (node != nullptr) {
					node->setPos(0, 0);
				}
			});

			nodePopup->show();
			break;
		}
		case Qt::Key_Backspace: {
			// Delete a node
			std::vector<GraphNode*> toDelete;
			for (auto n : m_operationNodes) {
				if (n.second->isSelected()) {
					toDelete.push_back(n.second);
				}
			}
			for (auto n : toDelete) {
				deleteOperationNode(n);
			}
			break;
		}
	};
}

void GraphScene::dragMoveEvent(QGraphicsSceneDragDropEvent* e)
{
	if (m_newEdge != nullptr) {
		m_newEdge->CursorPositionChanged(e->scenePos());
	}
	QGraphicsScene::dragMoveEvent(e);
}

}  // namespace pgeditor::gui::graph
