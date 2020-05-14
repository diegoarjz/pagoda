#include "router_node.h"

#include "input_interface_node.h"
#include "node_visitor.h"
#include "unsupported_node_link.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>
#include <pagoda/dynamic_value/string_value.h>
#include <pagoda/dynamic_value/type_info.h>
#include <pagoda/objects/procedural_object_predicate.h>
#include <pagoda/objects/procedural_object_predicate_registry.h>

#include "node_visitor.h"

namespace pagoda
{
using namespace objects;

const char *RouterNode::name = "Router";

RouterNode::RouterNode(ProceduralObjectPredicateRegistryPtr predicateRegistry) : m_predicateRegistry(predicateRegistry)
{
}
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
	out_visitor(ProceduralObjectPredicateRegistryPtr predicateRegistry, std::list<ProceduralObjectPtr> &objects,
	            std::shared_ptr<DynamicValueTable> &table)
	    : m_predicateRegistry(predicateRegistry), m_proceduralObjects(objects), m_nodeMemberTable(table)
	{
	}

	void Visit(std::shared_ptr<OperationNode> n) override { throw UnsupportedNodeLink("input", "OperationNode"); }

	void Visit(std::shared_ptr<InputInterfaceNode> n) override
	{
		auto nodeName = n->GetName();
		try
		{
			auto memberParameter = m_nodeMemberTable->Get(nodeName);
			if (memberParameter->GetTypeInfo() == String::s_typeInfo)
			{
				auto asString = std::dynamic_pointer_cast<String>(memberParameter);
				auto pred = m_predicateRegistry->Get(asString->ToString());

				if (pred == nullptr)
				{
					LOG_ERROR("Predicate with name " << asString->ToString() << " not found");
					return;
				}

				for (auto o : m_proceduralObjects)
				{
					if ((*pred)(o))
					{
						n->AddProceduralObject(o);
					}
				}
			}
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

	ProceduralObjectPredicateRegistryPtr m_predicateRegistry;
	std::list<ProceduralObjectPtr> &m_proceduralObjects;
	std::shared_ptr<DynamicValueTable> m_nodeMemberTable;
};
}  // namespace

void RouterNode::Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph, "Executing RouterNode " << GetName() << "(" << GetId() << ")");

	out_visitor v(m_predicateRegistry, m_proceduralObjects, m_memberTable);
	for (auto &i : outNodes)
	{
		i->AcceptNodeVisitor(&v);
	}
}

}  // namespace pagoda
