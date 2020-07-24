#pragma once
#ifndef PAGODA_PROCEDURAL_GRAPH_INPUT_INTERFACE_NODE_H_
#define PAGODA_PROCEDURAL_GRAPH_INPUT_INTERFACE_NODE_H_

#include "node.h"

#include <list>

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
	~InputInterfaceNode();

	void SetConstructionArguments(const std::unordered_map<std::string, dynamic::DynamicValueBasePtr>&) override;

	void Execute(const NodeSet& inNodes, const NodeSet& outNodes) override;
	void SetInterfaceName(const std::string& interfaceName);
	const std::string& GetInterfaceName() const;
	void AddProceduralObject(pagoda::objects::ProceduralObjectPtr object);
	void AcceptNodeVisitor(NodeVisitor* visitor) override;

	const std::list<pagoda::objects::ProceduralObjectPtr>& GetProceduralObjects() const { return m_proceduralObjects; }

private:
	std::string m_interfaceName;
	std::list<pagoda::objects::ProceduralObjectPtr> m_proceduralObjects;
};  // class OperationExecution
}  // namespace pagoda::graph

#endif
