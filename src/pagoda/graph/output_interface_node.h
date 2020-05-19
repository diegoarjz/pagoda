#pragma once
#ifndef PAGODA_PROCEDURAL_GRAPH_OUTPUT_INTERFACE_EXECUTION_H_
#define PAGODA_PROCEDURAL_GRAPH_OUTPUT_INTERFACE_EXECUTION_H_

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
class OutputInterfaceNode : public Node
{
public:
	static const char* name;

	OutputInterfaceNode();
	~OutputInterfaceNode();

	void SetConstructionArguments(const std::unordered_map<std::string, dynamic::DynamicValueBasePtr>&) override;

	void Execute(const NodeSet<Node>& inNodes, const NodeSet<Node>& outNodes) override;

	void SetInterfaceName(const std::string& name);
	const std::string& GetInterfaceName() const;
	const std::list<pagoda::objects::ProceduralObjectPtr>& GetProceduralObjects() const { return m_proceduralObjects; }
	void AddProceduralObject(pagoda::objects::ProceduralObjectPtr object);
	void AcceptNodeVisitor(NodeVisitor* visitor) override;

private:
	std::string m_interfaceName;
	std::list<pagoda::objects::ProceduralObjectPtr> m_proceduralObjects;
};  // class OutputInterfaceNode
}  // namespace pagoda::graph

#endif
