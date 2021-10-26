#include "output_interface_node.h"

#include "construction_argument_callback.h"
#include "construction_argument_not_found.h"
#include "graph.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"
#include "operation_node.h"
#include "unsupported_node_link.h"

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/objects/procedural_operation.h>

using namespace pagoda::dynamic;
using namespace pagoda::objects;

namespace pagoda::graph
{
const char* OutputInterfaceNode::name = "OutputInterface";

OutputInterfaceNode::OutputInterfaceNode() : m_interfaceName("", 0)
{
}
OutputInterfaceNode::~OutputInterfaceNode()
{
}

void OutputInterfaceNode::SetConstructionArguments(
  ConstructionArgumentCallback* cb)
{
	std::string interfaceName;
	cb->StringArgument("interface", interfaceName, "Output Interface");
	SetInterfaceName(interfaceName);
}

void OutputInterfaceNode::SetExecutionArguments(
  objects::NewParameterCallback* cb)
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

void OutputInterfaceNode::ForEachConstructionArgument(
  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
{
	f("interface", std::make_shared<dynamic::String>(m_interfaceName));
}

void OutputInterfaceNode::ForEachExecutionArgument(
  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
{
	//
}
}  // namespace pagoda::graph
