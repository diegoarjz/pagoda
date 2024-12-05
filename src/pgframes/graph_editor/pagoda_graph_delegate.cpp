#include "pagoda_graph_delegate.h"

#include "pagoda/graph/query/or.h"
#include "pgframes/viewer/viewer_node.h"

#include "pagoda/graph/graph.h"
#include "pagoda/graph/operation_node.h"
#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/output_interface_node.h"
#include "pagoda/graph/query/graph_grammar.h"
#include "pagoda/graph/query/rule.h"

#include "pagoda/graph/query/type.h"
#include "pagoda/objects/operation_factory.h"

#include "pagoda/pagoda.h"

using namespace pagoda;
using namespace pagoda::graph;

namespace pgframes::graph_editor{
  namespace {

  class EdgeLink : public query::Rule {
  public:
    EdgeLink(Graph &graph, GraphDelegate::EdgeList&linkInfo)
        : query::Rule(graph), m_topology(&m_upstreamNode), m_linkInfo{linkInfo} {
      m_topology.AddDownstreamQuery(&m_upstreamNode, &m_outputInterface);
      m_topology.AddDownstreamQuery(&m_outputInterface, &m_inputInterface);
      m_topology.AddDownstreamQuery(&m_inputInterface, &m_downstreamNode);
      m_downstreamNode.AddQuery(m_downstreamOpNode);
      m_downstreamNode.AddQuery(m_downstreamViewerNode);
    }

    query::QueryTopology &GetTopology() { return m_topology; }

    void ApplyMatch(const std::map<query::Query *, NodePtr> &match) {
      auto outInterface = std::dynamic_pointer_cast<OutputInterfaceNode>(
          match.at(&m_outputInterface));
      auto inInterface = std::dynamic_pointer_cast<InputInterfaceNode>(
          match.at(&m_inputInterface));

      GraphDelegate::Edge edge;
      std::size_t hash = 0;
      edge.output = reinterpret_cast<GraphDelegate::OutputHandle>(outInterface.get());
      edge.input = reinterpret_cast<GraphDelegate::InputHandle>(inInterface.get());
      m_linkInfo.push_back(edge);
    }

  private:
    query::Type<OperationNode> m_upstreamNode;
    query::Type<OutputInterfaceNode> m_outputInterface;
    query::Type<InputInterfaceNode> m_inputInterface;
    query::Or m_downstreamNode;
    query::Type<OperationNode> m_downstreamOpNode;
    query::Type<viewer::ViewerNode> m_downstreamViewerNode;
    query::QueryTopology m_topology;
    GraphDelegate::EdgeList &m_linkInfo;
  };
  }

  PagodaGraphDelegate::PagodaGraphDelegate(Pagoda& pagoda, GraphPtr& graph) : m_graph{graph}, m_pagoda{pagoda} {
    updateInternal();
  }

  PagodaGraphDelegate::~PagodaGraphDelegate() {}

  GraphDelegate::NodeHandleList PagodaGraphDelegate::Nodes() const {
    return m_nodes;
  }

  GraphDelegate::NodeHandle PagodaGraphDelegate::CreateNode(const std::string& type) {
    pagoda::graph::Graph::NodeIdentifier_t nodeId;
    if (type == "ViewerNode") {
      nodeId = m_graph->CreateNode(type);
    }
    else {
      nodeId = m_graph->CreateNode<OperationNode>(type);
      if (auto node = m_graph->GetNode(nodeId)) {
        if (auto opNode = std::dynamic_pointer_cast<OperationNode>(node)) {
          auto operation = m_pagoda.GetOperationFactory()->Create(type);
          opNode->SetOperation(operation);
        }
      }
    }

    updateInternal();
    return reinterpret_cast<NodeHandle>(m_graph->GetNode(nodeId).get());
  }

  void PagodaGraphDelegate::DeleteNode(const NodeHandle& n) {
    auto node = reinterpret_cast<pagoda::graph::Node*>(n);
    const auto nodeName = node->GetName();
    for (const auto &interface : m_graph->GetNodesAdjacentTo(nodeName)) {
      m_graph->DestroyNode(interface->GetName());
    }
    m_graph->DestroyNode(nodeName);
    updateInternal();
  }

  std::string PagodaGraphDelegate::NodeText(const NodeHandle& n) const {
    return reinterpret_cast<pagoda::graph::Node*>(n)->GetName();
  }

  void PagodaGraphDelegate::SetNodeText(const NodeHandle& n,const std::string& text) {
    const auto prevName = reinterpret_cast<pagoda::graph::Node*>(n)->GetName();
    m_graph->RenameNode(prevName, text);
  }

  void PagodaGraphDelegate::GetNodePosition(const NodeHandle& n, float& x, float &y) const {
    auto node = reinterpret_cast<pagoda::graph::Node*>(n);
    const auto pos = node->GetPos();
    x = pos.a[0];
    y = pos.a[1];
  }

  void PagodaGraphDelegate::SetNodePosition(const NodeHandle& n, float x, float y) {
    auto node = reinterpret_cast<pagoda::graph::Node*>(n);
    node->SetPos({x, y});
  }

  GraphDelegate::NodeInputList PagodaGraphDelegate::NodeInputs(const NodeHandle& n) const {
    auto node = reinterpret_cast<pagoda::graph::Node*>(n);
    auto inputs = m_graph->GetNodeInputNodes(node->GetName());
    NodeInputList inputList;
    for (const auto& i : inputs) {
      inputList.push_back(reinterpret_cast<InputHandle>(i.get()));
    }
    return inputList;
  }

  GraphDelegate::NodeOutputList PagodaGraphDelegate::NodeOutputs(const NodeHandle& n) const {
    auto node = reinterpret_cast<pagoda::graph::Node*>(n);
    auto outputs = m_graph->GetNodeOutputNodes(node->GetName());
    NodeOutputList outputList;
    for (const auto& i : outputs) {
      outputList.push_back(reinterpret_cast<InputHandle>(i.get()));
    }
    return outputList;
  }

  std::string PagodaGraphDelegate::InputText(const InputHandle& n) const {
    return reinterpret_cast<InputInterfaceNode*>(n)->GetInterfaceName();
  }

  std::string PagodaGraphDelegate::OutputText(const OutputHandle& n) const {
    return reinterpret_cast<OutputInterfaceNode*>(n)->GetInterfaceName();
  }

  GraphDelegate::EdgeList PagodaGraphDelegate::GraphEdges() {
    return m_edges;
  }

  bool PagodaGraphDelegate::CanCreateEdge(OutputHandle output, InputHandle input) const {
    return true;
  }

  void PagodaGraphDelegate::CreateEdge(OutputHandle output, InputHandle input) {
    m_graph->CreateEdge(reinterpret_cast<OutputInterfaceNode*>(output)->GetName(),
                        reinterpret_cast<InputInterfaceNode*>(input)->GetName());
    updateInternal();
  }

  void PagodaGraphDelegate::DeleteEdge(OutputHandle output, InputHandle input) {
    const auto out = reinterpret_cast<OutputInterfaceNode*>(output);
    const auto in = reinterpret_cast<InputInterfaceNode*>(input);
    m_graph->DestroyEdge(out->GetName(), in->GetName());
    updateInternal();
  }


  void PagodaGraphDelegate::updateInternal() {
    m_nodes.clear();
    m_edges.clear();

    auto addOpNode = [&](NodePtr n) {
      m_nodes.push_back(reinterpret_cast<NodeHandle>(n.get()));
    };
    auto opNodes = std::make_shared<query::Type<OperationNode>>();
    auto viewerNodes = std::make_shared<query::Type<viewer::ViewerNode>>();
    query::Or operationsOrViewer(*m_graph,
                                 addOpNode,
                                 {opNodes, viewerNodes});
    m_graph->ExecuteQuery(operationsOrViewer);

    query::GraphGrammar grammar(*m_graph);
    auto linker = std::make_shared<EdgeLink>(*m_graph, m_edges);
    grammar.AddRule(linker);
    grammar.Run();
  }
}
