#include "output_interface_node.h"

#include "graph.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"
#include "operation_node.h"
#include "unsupported_node_link.h"

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

void OutputInterfaceNode::AddProceduralObject(ProceduralObjectPtr object)
{
	m_proceduralObjects.push_back(object);
}

void OutputInterfaceNode::Execute(const NodeSet& inNodes,
                                  const NodeSet& outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph,
	          "Executing OutputInterfaceNode for interface " << m_interfaceName);
	LOG_TRACE(ProceduralGraph,
	          "  with " << m_proceduralObjects.size() << " objects.");

	for (auto& o : outNodes) {
		if (auto n = std::dynamic_pointer_cast<InputInterfaceNode>(o)) {
			for (auto& object : m_proceduralObjects) {
				n->AddProceduralObject(object);
			}
		}
	}
}

const char* const OutputInterfaceNode::GetNodeType()
{
	static const char* const sNodeType = "OutputInterface";
	return sNodeType;
}
}  // namespace pagoda::graph
