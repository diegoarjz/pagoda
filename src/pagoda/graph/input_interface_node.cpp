#include "input_interface_node.h"

#include "graph.h"
#include "node.h"
#include "node_set_visitor.h"
#include "operation_node.h"
#include "output_interface_node.h"
#include "unsupported_node_link.h"

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
}  // namespace pagoda::graph
