#pragma once

#include "node.h"

namespace pagoda::objects
{
using ProceduralOperationPtr = std::shared_ptr<class ProceduralOperation>;
using OperationFactoryPtr = std::shared_ptr<class OperationFactory>;
}  // namespace pagoda::objects

namespace pagoda::graph
{
class OperationNode : public Node
{
	public:
	static const char *name;

	OperationNode(pagoda::objects::OperationFactoryPtr operationFactory);
	~OperationNode();

	void SetConstructionArguments(ConstructionArgumentCallback *cb) override;
	void SetExecutionArguments(objects::NewParameterCallback *cb) override;

	void Execute(const NodeSet &inNodes, const NodeSet &outNodes) override;
	void SetOperation(pagoda::objects::ProceduralOperationPtr operation);
	pagoda::objects::ProceduralOperationPtr GetOperation() const
	{
		return m_operation;
	}
	void ForEachConstructionArgument(
	  std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> f)
	  override;
	void ForEachExecutionArgument(
	  std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> f)
	  override;

	void ForEachOperationParameter(
	  const std::function<void(const std::string &,
	                           const dynamic::DynamicValueBasePtr &)> &f) const;

	const char *const GetNodeType() override;

	private:
	pagoda::objects::ProceduralOperationPtr m_operation;
	pagoda::objects::OperationFactoryPtr m_operationFactory;
};  // class OperationNode
}  // namespace pagoda::graph

