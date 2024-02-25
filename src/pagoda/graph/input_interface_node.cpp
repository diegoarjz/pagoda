#include "input_interface_node.h"

#include "pagoda/graph/graph.h"
#include "pagoda/graph/node.h"
#include "pagoda/graph/operation_node.h"
#include "pagoda/graph/output_interface_node.h"
#include "pagoda/graph/operation_node.h"

#include <pagoda/dynamic/get_value_as.h>

#include <pagoda/objects/parameter.h>
#include <pagoda/objects/procedural_operation.h>

using namespace pagoda::dynamic;
using namespace pagoda::objects;

namespace pagoda::graph
{
const char* InputInterfaceNode::name = "InputInterface";

InputInterfaceNode::InputInterfaceNode() : m_interfaceName("", 0)
{
	m_parameters["interface"] =
	  std::make_shared<StringParameter>(&m_interfaceName, "interface");
}

InputInterfaceNode::~InputInterfaceNode()
{
}

void InputInterfaceNode::SetInterfaceName(const std::string& interfaceName)
{
	m_interfaceName = interfaceName;
}
const std::string& InputInterfaceNode::GetInterfaceName() const
{
	return m_interfaceName;
}

void InputInterfaceNode::Execute(const NodeSet& inNodes,
                                 const NodeSet& outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph,
	          "Executing InputInterfaceNode for interface " << m_interfaceName);
}

const char* const InputInterfaceNode::GetNodeType()
{
	static const char* const sNodeType = "InputInterface";
	return sNodeType;
}

void InputInterfaceNode::ConnectedToNode(const NodePtr& node) {
  Node::ConnectedToNode(node);

  if (auto output = std::dynamic_pointer_cast<OutputInterfaceNode>(node)) {
    LOG_TRACE(ProceduralGraph, "Connected Output Interface Node '" << output->GetName() 
        << "' with interface name '" << output->GetInterfaceName() << "' (" << output->GetInterface()
        << ") in Input Interface Node '" << GetName() << "' with interface name '"
        << GetInterfaceName() << "' (" << GetInterface() << ").");
    // Interface::Connect() doesn't create duplicate connections
    if (output->GetInterface() != nullptr && GetInterface() != nullptr) {
      if (!output->GetInterface()->Connect(GetInterface())) {
        LOG_ERROR("Unable to connect interface '" << GetInterfaceName() << "' of node '"
            << std::dynamic_pointer_cast<Node>(GetInterfaceableNode())->GetName() 
            << "' to interface '" << output->GetInterfaceName() << "' of node '"
            << std::dynamic_pointer_cast<Node>(output->GetInterfaceableNode())->GetName() << "'");
      }
    }
  }
  else if (auto opNode = std::dynamic_pointer_cast<InterfaceableNode>(node)) {
    LOG_TRACE(ProceduralGraph, "Connected Interfaceable Node '" << std::dynamic_pointer_cast<Node>(opNode)->GetName() 
        << "' in Input Interface Node '" << GetName() << "'.");
    m_interfaceableNode = opNode;
  }
}

void InputInterfaceNode::DisconnectedFromNode(const NodePtr& node) {
  Node::DisconnectedFromNode(node);

  if (auto output = std::dynamic_pointer_cast<OutputInterfaceNode>(node)) {
    output->GetInterface()->Disconnect(GetInterface());
  }
  else if (auto opNode = std::dynamic_pointer_cast<InterfaceableNode>(node)) {
    m_interfaceableNode.reset();
  }
}

InterfaceableNodePtr InputInterfaceNode::GetInterfaceableNode() const {
  if (m_interfaceableNode.expired()) {
    LOG_WARNING("Trying to get the InterfaceableNode from an InputInterfaceNode but it is not conneted to an InterfaceableNode.")
    return nullptr;
  }
  return m_interfaceableNode.lock();
}

objects::InterfacePtr InputInterfaceNode::GetInterface() const {
  if (m_interfaceableNode.expired()) {
    LOG_WARNING("Trying to get the interface from an InputInterfaceNode but it is not connected to an InterfaceableNode.")
    return nullptr;
  }
  return GetInterfaceableNode()->GetInputInterface(m_interfaceName);
}

}  // namespace pagoda::graph
