#include "node_link_definition.h"

#include "ast_node_visitor.h"

namespace pagoda::graph::io
{
NodeLinkDefinition::NodeLinkDefinition() : m_inputInterface(""), m_outputInterface(""), m_nodeName("") {}
NodeLinkDefinition::NodeLinkDefinition(const std::string& node, const std::string& inInterface,
                                       const std::string& outInterface)
  : m_inputInterface(inInterface), m_outputInterface(outInterface), m_nodeName(node)
{
}

NodeLinkDefinition::~NodeLinkDefinition() {}

const std::string& NodeLinkDefinition::GetInputInterface() const { return m_inputInterface; }
const std::string& NodeLinkDefinition::GetOutputInterface() const { return m_outputInterface; }
const std::string& NodeLinkDefinition::GetNodeName() const { return m_nodeName; }
void NodeLinkDefinition::SetInputInterface(const std::string& name) { m_inputInterface = name; }
void NodeLinkDefinition::SetOutputInterface(const std::string& name) { m_outputInterface = name; }
void NodeLinkDefinition::SetNodeName(const std::string& name) { m_nodeName = name; }

const std::string NodeLinkDefinition::GetInNode() const
{
	if (m_inputInterface.empty()) {
		return m_nodeName;
	}
	return m_nodeName + "_" + m_inputInterface;
}
const std::string NodeLinkDefinition::GetOutNode() const
{
	if (m_outputInterface.empty()) {
		return m_nodeName;
	}
	return m_nodeName + "_" + m_outputInterface;
}

void NodeLinkDefinition::AcceptVisitor(AstNodeVisitor* visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<NodeLinkDefinition>(shared_from_this()).get());
}

}  // namespace pagoda::graph::io
