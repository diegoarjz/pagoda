#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace pgframes::graph_editor {
class GraphDelegate
{
public:
  using NodeHandle = uint64_t;
  using NodeHandleList = std::vector<NodeHandle>;
  using InputHandle = uint64_t;
  using OutputHandle = uint64_t;
  using NodeInputList = std::vector<InputHandle>;
  using NodeOutputList = std::vector<OutputHandle>;

  struct Edge {
    OutputHandle output;
    InputHandle input;
  };
  using EdgeList = std::vector<Edge>;

  virtual ~GraphDelegate() {};

  virtual NodeHandleList Nodes() const = 0;
  virtual NodeHandle CreateNode(const std::string& type) = 0;
  virtual void DeleteNode(const NodeHandle&) = 0;

  virtual std::string NodeText(const NodeHandle&) const = 0;
  virtual void SetNodeText(const NodeHandle&, const std::string&) = 0;
  virtual void GetNodePosition(const NodeHandle&, float& x, float &y) const = 0;
  virtual void SetNodePosition(const NodeHandle&, float x, float y) = 0;

  virtual NodeInputList NodeInputs(const NodeHandle&) const = 0;
  virtual NodeOutputList NodeOutputs(const NodeHandle&) const = 0;
  virtual std::string InputText(const InputHandle&) const = 0;
  virtual std::string OutputText(const OutputHandle&) const = 0;

  virtual EdgeList GraphEdges() = 0;
  virtual bool CanCreateEdge(OutputHandle output, InputHandle input) const = 0;
  virtual void CreateEdge(OutputHandle output, InputHandle input) = 0;
  virtual void DeleteEdge(OutputHandle output, InputHandle input) = 0;
};

}
