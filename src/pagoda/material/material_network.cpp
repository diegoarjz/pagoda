#include "material_network.h"

#include <boost/functional/hash.hpp>

namespace pagoda::material {

MaterialNetwork::MaterialNetwork(const std::string &materialName)
    : m_materialName{materialName} {}
MaterialNetwork::~MaterialNetwork() {}

const std::string &MaterialNetwork::GetName() const { return m_materialName; }

MaterialNode *MaterialNetwork::CreateMaterialNode(const std::string &nodeId,
                                                  const std::string &name) {
  if (m_nodes.find(name) == m_nodes.end()) {
    MaterialNode m{nodeId, name};
    auto iter = m_nodes.emplace(name, std::move(m));
    return &(iter.first->second);
  }
  return nullptr;
}

const MaterialNode *
MaterialNetwork::GetMaterialNode(const std::string &name) const {
  auto iter = m_nodes.find(name);
  if (iter == m_nodes.end()) {
    return nullptr;
  }
  return &iter->second;
}

MaterialNode *MaterialNetwork::GetMaterialNode(const std::string &name) {
  auto iter = m_nodes.find(name);
  if (iter == m_nodes.end()) {
    return nullptr;
  }
  return &iter->second;
}

void MaterialNetwork::SetStageTerminalNode(ShaderStage stage,
                                           const std::string &name) {
  switch (stage) {
  case ShaderStage::Vertex:
    m_terminalNodes[0] = name;
    break;
  case ShaderStage::Fragment:
    m_terminalNodes[1] = name;
    break;
  }
}
MaterialNode *MaterialNetwork::GetStageTerminalNode(ShaderStage stage) {
  switch (stage) {
  case ShaderStage::Vertex:
    return GetMaterialNode(m_terminalNodes[0]);
    break;
  case ShaderStage::Fragment:
    return GetMaterialNode(m_terminalNodes[1]);
    break;
  }
}

void MaterialNetwork::AppendHash(std::size_t &hash) const {
  for (const auto &node : m_nodes) {
    node.second.AppendHash(hash);
  }
  boost::hash_combine(hash, m_terminalNodes[0]);
  boost::hash_combine(hash, m_terminalNodes[1]);
}

} // namespace pagoda::material
