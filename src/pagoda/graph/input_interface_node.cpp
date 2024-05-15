#include "input_interface_node.h"

#include "pagoda/graph/graph.h"
#include "pagoda/graph/node.h"
#include "pagoda/graph/node_set_visitor.h"
#include "pagoda/graph/operation_node.h"
#include "pagoda/graph/output_interface_node.h"
#include "pagoda/graph/unsupported_node_link.h"

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
}  // namespace pagoda::graph
