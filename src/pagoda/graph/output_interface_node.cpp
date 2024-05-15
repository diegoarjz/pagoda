#include "pagoda/graph/output_interface_node.h"

#include "pagoda/graph/graph.h"
#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/node.h"
#include "pagoda/graph/node_set_visitor.h"
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
}  // namespace pagoda::graph
