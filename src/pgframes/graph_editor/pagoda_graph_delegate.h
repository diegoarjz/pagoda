#pragma once

#include "graph_delegate.h"

#include <memory>

namespace pagoda {
  namespace graph {
    using GraphPtr = std::shared_ptr<class Graph>;
  }
  class Pagoda;
}

namespace pgframes::graph_editor {
class PagodaGraphDelegate : public GraphDelegate
{
public:
  PagodaGraphDelegate(pagoda::Pagoda& pagoda, pagoda::graph::GraphPtr& graph);

  ~PagodaGraphDelegate() override;

  NodeHandleList Nodes() const override;
  NodeHandle CreateNode(const std::string& type) override;
  void DeleteNode(const NodeHandle& n) override;

  std::string NodeText(const NodeHandle& n) const override;
  void SetNodeText(const NodeHandle&,const std::string&) override;
  void GetNodePosition(const NodeHandle& n, float& x, float &y) const override;
  void SetNodePosition(const NodeHandle&, float x, float y) override;

  NodeInputList NodeInputs(const NodeHandle& n) const override;
  NodeOutputList NodeOutputs(const NodeHandle& n) const override;
  std::string InputText(const InputHandle& n) const override;
  std::string OutputText(const OutputHandle& n) const override;

  EdgeList GraphEdges() override;
  bool CanCreateEdge(OutputHandle output, InputHandle input) const override;
  void CreateEdge(OutputHandle output, InputHandle input) override;
  void DeleteEdge(OutputHandle output, InputHandle input) override;

private:
  void updateInternal();

  NodeHandleList m_nodes;
  EdgeList m_edges;
  pagoda::graph::GraphPtr m_graph;
  pagoda::Pagoda& m_pagoda;
};
}
