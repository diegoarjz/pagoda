#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_INPUT_INTERFACE_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_INPUT_INTERFACE_NODE_H_

#include "node.h"

#include <list>

namespace selector
{
struct InterfaceName;

class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;

class InputInterfaceNode : public Node
{
public:
	static const char* name;

	InputInterfaceNode();
	~InputInterfaceNode();

	void SetConstructionArguments(const std::unordered_map<std::string, DynamicValueBasePtr>&) override;

	void Execute(const NodeSet<Node>& inNodes, const NodeSet<Node>& outNodes) override;
	void SetInterfaceName(const InterfaceName& interfaceName);
	const InterfaceName& GetInterfaceName() const;
	void AddProceduralObject(ProceduralObjectPtr object);
	void AcceptNodeVisitor(NodeVisitor* visitor) override;

	const std::list<ProceduralObjectPtr>& GetProceduralObjects() const { return m_proceduralObjects; }

private:
	InterfaceName m_interfaceName;
	std::list<ProceduralObjectPtr> m_proceduralObjects;
};  // class OperationExecution
}  // namespace selector

#endif
