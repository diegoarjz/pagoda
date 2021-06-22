#include "operation_node.h"

#include "graph_in_port.h"
#include "graph_out_port.h"
#include "node_style.h"

#include <pagoda/graph/query/type.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>

using namespace pagoda::graph;
using namespace pagoda::graph::query;

namespace pgeditor::gui::graph
{
OperationNode::OperationNode(pagoda::graph::GraphPtr graph, pagoda::graph::NodePtr node) : GraphNode(graph, node) {}

OperationNode::~OperationNode() {}

void OperationNode::CreateBody()
{
	using namespace node_style::header;
	using namespace node_style::node;

	m_inNodes = m_graph->GetNodeInputNodes(m_node->GetName());
	m_outNodes = m_graph->GetNodeOutputNodes(m_node->GetName());

	NodeSet inputInterfaces;
	auto inputFilter = query::type<InputInterfaceNode>(*m_graph, inputInterfaces);
	m_inNodes.ExecuteQuery(inputFilter);

	for (auto n : inputInterfaces) {
		auto interface = std::dynamic_pointer_cast<InputInterfaceNode>(n);
		auto proxy = new GraphInPort(this, interface, this);
		AddPort(proxy);
		m_inInterfaces.emplace(n, proxy);
	}

	NodeSet outputInterfaces;
	auto outputFilter = query::type<OutputInterfaceNode>(*m_graph, outputInterfaces);
	m_outNodes.ExecuteQuery(outputFilter);
	for (auto n : outputInterfaces) {
		auto interface = std::dynamic_pointer_cast<OutputInterfaceNode>(n);
		auto proxy = new GraphOutPort(this, interface, this);
		AddPort(proxy);
		m_outInterfaces.emplace(n, proxy);
	}
}

GraphInPort* OperationNode::GetInPortForNode(NodePtr node) const
{
	auto iter = m_inInterfaces.find(node);
	if (iter == m_inInterfaces.end()) {
		return nullptr;
	}
	return iter->second;
}

GraphOutPort* OperationNode::GetOutPortForNode(NodePtr node) const
{
	auto iter = m_outInterfaces.find(node);
	if (iter == m_outInterfaces.end()) {
		return nullptr;
	}
	return iter->second;
}
}  // namespace pgeditor::gui::graph
