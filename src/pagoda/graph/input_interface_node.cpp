#include "input_interface_node.h"

#include "construction_argument_callback.h"
#include "graph.h"
#include "node.h"
#include "node_set_visitor.h"
#include "operation_node.h"
#include "output_interface_node.h"
#include "unsupported_node_link.h"

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/objects/procedural_operation.h>

using namespace pagoda::dynamic;
using namespace pagoda::objects;

namespace pagoda::graph
{
const char* InputInterfaceNode::name = "InputInterface";

InputInterfaceNode::InputInterfaceNode() : m_interfaceName("", 0)
{
}
InputInterfaceNode::~InputInterfaceNode()
{
}

void InputInterfaceNode::SetConstructionArguments(
  ConstructionArgumentCallback* cb)
{
	std::string interfaceName;
	cb->StringArgument("interface", interfaceName, "Interface Name");
	SetInterfaceName(interfaceName);
}

void InputInterfaceNode::SetExecutionArguments(ExecutionArgumentCallback* cb)
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

void InputInterfaceNode::AddProceduralObject(ProceduralObjectPtr object)
{
	m_proceduralObjects.push_back(object);
}

void InputInterfaceNode::Execute(const NodeSet& inNodes,
                                 const NodeSet& outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph,
	          "Executing InputInterfaceNode for interface " << m_interfaceName);
	LOG_TRACE(ProceduralGraph,
	          "  with " << m_proceduralObjects.size() << " objects.");
}

const char* const InputInterfaceNode::GetNodeType()
{
	static const char* const sNodeType = "InputInterface";
	return sNodeType;
}

void InputInterfaceNode::ForEachConstructionArgument(
  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
{
	f("interface", std::make_shared<dynamic::String>(m_interfaceName));
}

void InputInterfaceNode::ForEachExecutionArgument(
  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
{
}
}  // namespace pagoda::graph
