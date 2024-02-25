#include "pagoda/graph/output_interface_node.h"

#include "pagoda/graph/graph.h"
#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/node.h"
#include "pagoda/graph/operation_node.h"
#include "pagoda/graph/unsupported_node_link.h"

#include <pagoda/dynamic/get_value_as.h>

#include <pagoda/objects/parameter.h>
#include <pagoda/objects/procedural_operation.h>

using namespace pagoda::dynamic;
using namespace pagoda::objects;

namespace pagoda::graph
{
const char* OutputInterfaceNode::name = "OutputInterface";

OutputInterfaceNode::OutputInterfaceNode() : m_interfaceName("", 0)
{
	m_parameters["interface"] =
	  std::make_shared<StringParameter>(&m_interfaceName, "interface");
}

OutputInterfaceNode::~OutputInterfaceNode()
{
}

void OutputInterfaceNode::SetInterfaceName(const std::string& name)
{
	m_interfaceName = name;
}
const std::string& OutputInterfaceNode::GetInterfaceName() const
{
	return m_interfaceName;
}

void OutputInterfaceNode::Execute(const NodeSet& inNodes,
                                  const NodeSet& outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph,
	          "Executing OutputInterfaceNode for interface " << m_interfaceName);
}

const char* const OutputInterfaceNode::GetNodeType()
{
	static const char* const sNodeType = "OutputInterface";
	return sNodeType;
}

void OutputInterfaceNode::ConnectedToNode(const NodePtr& node) {
  Node::ConnectedToNode(node);

  if (auto input = std::dynamic_pointer_cast<InputInterfaceNode>(node)) {
    LOG_TRACE(ProceduralGraph, "Connected Output Interface Node '" << input->GetName() 
        << "' with interface name '" << input->GetInterfaceName() << "' (" << input->GetInterface()
        << ") in Output Interface Node '" << GetName() << "' with interface name '"
        << GetInterfaceName() << "' (" << GetInterface() << ".");
    if (input->GetInterface() != nullptr && GetInterface() != nullptr) {
      // Interface::Connect() doesn't create duplicate connections
      if (!input->GetInterface()->Connect(GetInterface())) {
        LOG_ERROR("Unable to connect interface '" << GetInterfaceName() << "' of node '"
            << std::dynamic_pointer_cast<Node>(GetInterfaceableNode())->GetName()
            << "' to interface '" << input->GetInterfaceName() << "' of node '"
            << std::dynamic_pointer_cast<Node>(input->GetInterfaceableNode())->GetName() << "'");
      }
    }
  }
  else if (auto opNode = std::dynamic_pointer_cast<InterfaceableNode>(node)) {
    m_interfaceableNode = opNode;
  }
}

void OutputInterfaceNode::DisconnectedFromNode(const NodePtr& node) {
  Node::DisconnectedFromNode(node);

  if (auto input = std::dynamic_pointer_cast<InputInterfaceNode>(node)) {
    input->GetInterface()->Disconnect(GetInterface());
  }
  else if (auto opNode = std::dynamic_pointer_cast<InterfaceableNode>(node)) {
    m_interfaceableNode.reset();
  }
}

InterfaceableNodePtr OutputInterfaceNode::GetInterfaceableNode() const {
  if (m_interfaceableNode.expired()) {
    LOG_WARNING("Trying to get the Interfaceable Node from an OutputInterfaceNode but it is not conneted to an Interfaceable Node.")
    return nullptr;
  }
  return m_interfaceableNode.lock();
}

objects::InterfacePtr OutputInterfaceNode::GetInterface() const {
  if (m_interfaceableNode.expired()) {
    LOG_WARNING("Trying to get the interface from an OutputInterfaceNode but it is not connected to an Interfaceable Node.")
    return nullptr;
  }
  return GetInterfaceableNode()->GetOutputInterface(m_interfaceName);
}
}  // namespace pagoda::graph
