#include "parameter_input_node.h"

#include "construction_argument_callback.h"
#include "construction_argument_not_found.h"

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/string_value.h>

#include <iostream>

namespace pagoda::graph
{
ParameterInputNode::ParameterInputNode()
{
}
ParameterInputNode::~ParameterInputNode()
{
}

void ParameterInputNode::SetConstructionArguments(
  ConstructionArgumentCallback* cb)
{
	cb->StringArgument("name", m_parameterName, "Parameter Name");
}
void ParameterInputNode::SetExecutionArguments(objects::ParameterCallback* cb)
{
}

void ParameterInputNode::Execute(const NodeSet& inNodes,
                                 const NodeSet& outNodes)
{
	std::cout << "Executing ParameterInputNode" << std::endl;
	//
}

void ParameterInputNode::ForEachConstructionArgument(
  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
{
	f("name", std::make_shared<dynamic::String>(m_parameterName));
}

void ParameterInputNode::ForEachExecutionArgument(
  std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f)
{
	//
}

const std::string& ParameterInputNode::GetParameterName() const
{
	return m_parameterName;
}

const char* const ParameterInputNode::GetNodeType()
{
	static const char* const sNodeType = "ParameterInput";
	return sNodeType;
}
}  // namespace pagoda::graph
