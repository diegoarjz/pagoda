#pragma once

#include "pagoda/graph/node.h"
#include "pagoda/graph/interfaceable_node.h"

namespace pagoda::objects
{
using ProceduralOperationPtr = std::shared_ptr<class ProceduralOperation>;
using OperationFactoryPtr = std::shared_ptr<class OperationFactory>;

using InterfacePtr = std::shared_ptr<Interface>;
using InterfacePtrMap = std::unordered_map<std::string, InterfacePtr>;
}  // namespace pagoda::objects

namespace pagoda::graph
{
class OperationNode : public Node, public InterfaceableNode
{
	public:
	static const char *name;

	OperationNode(pagoda::objects::OperationFactoryPtr operationFactory);
	~OperationNode();

	void Execute(const NodeSet &inNodes, const NodeSet &outNodes) override;
	void SetOperation(pagoda::objects::ProceduralOperationPtr operation);
	pagoda::objects::ProceduralOperationPtr GetOperation() const
	{
		return m_operation;
	}

	void ForEachOperationParameter(
	  const std::function<void(const std::string &,
	                           const dynamic::DynamicValueBasePtr &)> &f) const;

	const char *const GetNodeType() override;

  void Interfaces(objects::InterfaceCallback* cb) override;
  void InputInterfaces(objects::InterfaceCallback* cb) override;
  void OutputInterfaces(objects::InterfaceCallback* cb) override;
  objects::InterfacePtr GetInputInterface(const std::string& name) const override;
  objects::InterfacePtr GetOutputInterface(const std::string& name) const override;

	private:
	objects::ProceduralOperationPtr m_operation;
	objects::OperationFactoryPtr m_operationFactory;
	std::string m_operationName;
};  // class OperationNode
}  // namespace pagoda::graph

