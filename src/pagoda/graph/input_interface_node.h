#pragma once

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
	~InputInterfaceNode() override;

	void SetConstructionArguments(ConstructionArgumentCallback* cb) override;
	void SetExecutionArguments(objects::NewParameterCallback* cb) override;

	void Execute(const NodeSet& inNodes, const NodeSet& outNodes) override;
	void SetInterfaceName(const std::string& interfaceName);
	const std::string& GetInterfaceName() const;
	void AddProceduralObject(pagoda::objects::ProceduralObjectPtr object);

	void ForEachConstructionArgument(
	  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
	  override;

	void ForEachExecutionArgument(
	  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
	  override;

	const std::list<pagoda::objects::ProceduralObjectPtr>& GetProceduralObjects()
	  const
	{
		return m_proceduralObjects;
	}

	const char* const GetNodeType() override;

	private:
	std::string m_interfaceName;
	std::list<pagoda::objects::ProceduralObjectPtr> m_proceduralObjects;
};  // class OperationExecution

using InputInterfaceNodePtr = std::shared_ptr<InputInterfaceNode>;
}  // namespace pagoda::graph
