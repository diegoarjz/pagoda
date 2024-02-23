#pragma once

#include "pagoda/graph/node.h"

namespace pagoda::objects
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
}  // namespace pagoda::objects

namespace pagoda::graph
{
class InputInterfaceNode : public Node
{
	public:
	static const char* name;

	InputInterfaceNode();
	~InputInterfaceNode() override;

	void Execute(const NodeSet& inNodes, const NodeSet& outNodes) override;
	void SetInterfaceName(const std::string& interfaceName);
	const std::string& GetInterfaceName() const;

	const char* const GetNodeType() override;

	private:
	std::string m_interfaceName;
};  // class OperationExecution

using InputInterfaceNodePtr = std::shared_ptr<InputInterfaceNode>;
}  // namespace pagoda::graph
