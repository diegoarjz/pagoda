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

	void SetConstructionArguments(ConstructionArgumentCallback* cb) override;
	void SetExecutionArguments(objects::NewParameterCallback* cb) override;

	void Execute(const NodeSet& inNodes, const NodeSet& outNodes) override;

	void SetInterfaceName(const std::string& name);
	const std::string& GetInterfaceName() const;
	const std::list<pagoda::objects::ProceduralObjectPtr>& GetProceduralObjects()
	  const
	{
		return m_proceduralObjects;
	}
	void AddProceduralObject(pagoda::objects::ProceduralObjectPtr object);
	void ForEachConstructionArgument(
	  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
	  override;
	void ForEachExecutionArgument(
	  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
	  override;

	const char* const GetNodeType() override;

	private:
	std::string m_interfaceName;
	std::list<pagoda::objects::ProceduralObjectPtr> m_proceduralObjects;
};  // class OutputInterfaceNode

using OutputInterfaceNodePtr = std::shared_ptr<OutputInterfaceNode>;
}  // namespace pagoda::graph

#endif
