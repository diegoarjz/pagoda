#include "pagoda/graph/node.h"

#include <pagoda/objects/parameter.h>

#include <pagoda/common/debug/assertions.h>
#include <pagoda/dynamic/expression.h>
#include <pagoda/dynamic/type_info.h>
#include <pagoda/dynamic/value_not_found.h>

using namespace pagoda::dynamic;

namespace pagoda::graph {
const TypeInfoPtr Node::s_typeInfo = std::make_shared<TypeInfo>("Node");

Node::Node() : BuiltinClass(s_typeInfo), m_nodeName(""), m_nodeId(0) {
  m_parameters["posX"] =
      std::make_shared<objects::FloatParameter>(&m_posX, "posX");
  m_parameters["posY"] =
      std::make_shared<objects::FloatParameter>(&m_posY, "posY");
}
Node::~Node() {}

uint32_t Node::GetId() const { return m_nodeId; }
void Node::SetId(uint32_t nodeId) { m_nodeId = nodeId; }

void Node::SetName(const std::string &name) { m_nodeName = name; }
const std::string &Node::GetName() const { return m_nodeName; }

void Node::AttachToGraph(Graph* graph) {
  m_graph = graph;
}

std::string Node::ToString() const { return "<Node>"; }

void Node::AcceptVisitor(ValueVisitorBase &visitor) {
  throw common::exception::Exception("Unimplemented");
}

objects::ParameterBasePtr Node::GetParameter(const std::string &name) const {
  auto iter = m_parameters.find(name);
  if (iter == m_parameters.end()) {
    return nullptr;
  }
  return iter->second;
}

void Node::ForEachParameter(
    std::function<void(objects::ParameterBasePtr)> f) const {
  for (const auto &p : m_parameters) {
    f(p.second);
  }
}

void Node::SetPos(const math::Vec2F pos) {
  if (auto p = std::dynamic_pointer_cast<objects::FloatParameter>(
          GetParameter("posX"))) {
    p->SetValue(boost::qvm::X(pos));
  }
  if (auto p = std::dynamic_pointer_cast<objects::FloatParameter>(
          GetParameter("posY"))) {
    p->SetValue(boost::qvm::Y(pos));
  }
}

math::Vec2F Node::GetPos() const { return {m_posX, m_posY}; }

} // namespace pagoda::graph
