#pragma once

#include "node.h"

namespace pagoda::dynamic
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
}  // namespace pagoda::dynamic

namespace pagoda::graph
{
/**
 * Node to propagate parameters to its out nodes.
 * All parameters in defined in this node are set in the parameter context of
 * each of the output nodes overwriting them.
 */
class ParameterNode : public Node
{
	public:
	static const char *name;

	ParameterNode();
	virtual ~ParameterNode();

	void SetConstructionArguments(ConstructionArgumentCallback *cb) override;
	void SetExecutionArguments(objects::ParameterCallback *cb) override;
	void ForEachConstructionArgument(
	  std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> f)
	  override;
	void ForEachExecutionArgument(
	  std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> f)
	  override;

	void Execute(const NodeSet &inNodes, const NodeSet &outNodes) override;

	const char *const GetNodeType() override;
};

}  // namespace pagoda::graph
