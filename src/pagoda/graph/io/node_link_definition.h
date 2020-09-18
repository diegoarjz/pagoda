#pragma once

#include "ast_node.h"

#include <string>

namespace pagoda::graph::io
{
class NodeLinkDefinition : public AstNode
{
	public:
	NodeLinkDefinition();
	NodeLinkDefinition(const std::string& node, const std::string& inInterface, const std::string& outInterface);
	~NodeLinkDefinition() override;

	const std::string& GetInputInterface() const;
	const std::string& GetOutputInterface() const;
	const std::string& GetNodeName() const;
	void SetInputInterface(const std::string& name);
	void SetOutputInterface(const std::string& name);
	void SetNodeName(const std::string& name);

	const std::string GetInNode() const;
	const std::string GetOutNode() const;

	void AcceptVisitor(AstNodeVisitor* visitor) override;

	private:
	std::string m_inputInterface;
	std::string m_outputInterface;
	std::string m_nodeName;
};
}  // namespace pagoda::graph::io
