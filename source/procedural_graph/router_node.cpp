#include "router_node.h"

#include "common/assertions.h"
#include "common/logger.h"
#include "common/profiler.h"
#include "input_interface_node.h"
#include "node_visitor.h"
#include "unsupported_node_link.h"

#include "node_visitor.h"

namespace selector
{
const char *RouterNode::name = "Router";

RouterNode::RouterNode() {}
RouterNode::~RouterNode() {}

void RouterNode::SetConstructionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &) {}

void RouterNode::AcceptNodeVisitor(NodeVisitor *visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<RouterNode>(shared_from_this()));
}

void RouterNode::AddProceduralObject(ProceduralObjectPtr object) { m_proceduralObjects.push_back(object); }

namespace
{
class out_visitor : public NodeVisitor
{
public:
	out_visitor(std::list<ProceduralObjectPtr> &objects, std::shared_ptr<DynamicValueTable> &table)
	    : m_proceduralObjects(objects), m_nodeMemberTable(table)
	{
	}

	void Visit(std::shared_ptr<OperationNode> n) override { throw UnsupportedNodeLink("input", "OperationNode"); }

	void Visit(std::shared_ptr<InputInterfaceNode> n) override
	{
		auto nodeName = n->GetName();
		try
		{
			auto memberParameter = m_nodeMemberTable->Get(nodeName);
		}
		catch (...)
		{
			// Nothing mapped to the out node
			return;
		}
	}

	void Visit(std::shared_ptr<OutputInterfaceNode> n) override
	{
		throw UnsupportedNodeLink("input", "OutputInterfaceNode");
	}

	void Visit(std::shared_ptr<ParameterNode> n) override { throw UnsupportedNodeLink("input", "ParameterNode"); }

	void Visit(std::shared_ptr<RouterNode> n) override { throw UnsupportedNodeLink("input", "RouterNode"); }

	std::list<ProceduralObjectPtr> &m_proceduralObjects;
	std::shared_ptr<DynamicValueTable> m_nodeMemberTable;
};
}  // namespace

void RouterNode::Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph, "Executing RouterNode " << GetName() << "(" << GetId() << ")");

	out_visitor v(m_proceduralObjects, m_memberTable);
	for (auto &i : outNodes)
	{
		i->AcceptNodeVisitor(&v);
	}
}

}  // namespace selector
