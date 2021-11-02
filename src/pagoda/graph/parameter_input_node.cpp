#include "parameter_input_node.h"

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

void ParameterInputNode::Execute(const NodeSet& inNodes,
                                 const NodeSet& outNodes)
{
	std::cout << "Executing ParameterInputNode" << std::endl;
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
