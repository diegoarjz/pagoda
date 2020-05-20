#include "input_interface_node.h"

#include "construction_argument_not_found.h"
#include "graph.h"
#include "node.h"
#include "node_set_visitor.h"
#include "node_visitor.h"
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

InputInterfaceNode::InputInterfaceNode() : m_interfaceName("", 0) {}
InputInterfaceNode::~InputInterfaceNode() {}

void InputInterfaceNode::SetConstructionArguments(
    const std::unordered_map<std::string, DynamicValueBasePtr>& constructionArgs)
{
	auto interfaceNameIter = constructionArgs.find("interface");
	if (interfaceNameIter == std::end(constructionArgs))
	{
		throw ConstructionArgumentNotFound(GetName(), GetId(), "interface");
	}

	auto interfaceName = get_value_as<std::string>(*(interfaceNameIter->second));
	SetInterfaceName(interfaceName);
}

void InputInterfaceNode::SetInterfaceName(const std::string& interfaceName) { m_interfaceName = interfaceName; }
const std::string& InputInterfaceNode::GetInterfaceName() const { return m_interfaceName; }

void InputInterfaceNode::AddProceduralObject(ProceduralObjectPtr object) { m_proceduralObjects.push_back(object); }

void InputInterfaceNode::AcceptNodeVisitor(NodeVisitor* visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<InputInterfaceNode>(shared_from_this()));
}

namespace
{
class out_visitor : public NodeVisitor
{
public:
	out_visitor(const std::string& name, std::list<ProceduralObjectPtr>& objects)
	    : m_interfaceName(name), m_proceduralObjects(objects)
	{
	}

	void Visit(std::shared_ptr<OperationNode> n) override
	{
		ProceduralOperationPtr op = n->GetOperation();
		for (auto object : m_proceduralObjects)
		{
			op->PushProceduralObject(m_interfaceName, object);
		}
	}

	void Visit(std::shared_ptr<InputInterfaceNode> n) override
	{
		throw UnsupportedNodeLink("input", "InputInterfaceNode");
	}

	void Visit(std::shared_ptr<OutputInterfaceNode> n) override
	{
		throw UnsupportedNodeLink("input", "OutputInterfaceNode");
	}

	void Visit(std::shared_ptr<ParameterNode> n) override { throw UnsupportedNodeLink("input", "ParameterNode"); }

	void Visit(std::shared_ptr<RouterNode> n) override { throw UnsupportedNodeLink("input", "RouterNode"); }

	const std::string& m_interfaceName;
	std::list<ProceduralObjectPtr>& m_proceduralObjects;
};
}  // namespace

void InputInterfaceNode::Execute(const NodeSet<Node>& inNodes, const NodeSet<Node>& outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph, "Executing InputInterfaceNode " << GetName() << "(" << GetId() << ")");

	out_visitor v(m_interfaceName, m_proceduralObjects);
	for (auto& i : outNodes)
	{
		i->AcceptNodeVisitor(&v);
	}
}
}  // namespace pagoda::graph
