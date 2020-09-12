#include "output_interface_node.h"

#include "construction_argument_not_found.h"
#include "graph.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"
#include "node_visitor.h"
#include "operation_node.h"
#include "unsupported_node_link.h"

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/objects/procedural_operation.h>

using namespace pagoda::dynamic;
using namespace pagoda::objects;

namespace pagoda::graph
{
const char* OutputInterfaceNode::name = "OutputInterface";

OutputInterfaceNode::OutputInterfaceNode() : m_interfaceName("", 0) {}
OutputInterfaceNode::~OutputInterfaceNode() {}

void OutputInterfaceNode::SetConstructionArguments(
  const std::unordered_map<std::string, DynamicValueBasePtr>& constructionArgs)
{
	auto interfaceNameIter = constructionArgs.find("interface");
	if (interfaceNameIter == std::end(constructionArgs)) {
		throw ConstructionArgumentNotFound(GetName(), GetId(), "interface");
	}

	auto interfaceName = get_value_as<std::string>(*interfaceNameIter->second);
	SetInterfaceName(interfaceName);
}

void OutputInterfaceNode::SetInterfaceName(const std::string& name) { m_interfaceName = name; }
const std::string& OutputInterfaceNode::GetInterfaceName() const { return m_interfaceName; }

void OutputInterfaceNode::AcceptNodeVisitor(NodeVisitor* visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<OutputInterfaceNode>(shared_from_this()));
}

void OutputInterfaceNode::AddProceduralObject(ProceduralObjectPtr object) { m_proceduralObjects.push_back(object); }

void OutputInterfaceNode::Execute(const NodeSet& inNodes, const NodeSet& outNodes) { START_PROFILE; }

}  // namespace pagoda::graph
