#pragma once

#include <memory>

namespace selector
{
class OperationNode;
class InputInterfaceNode;
class OutputInterfaceNode;
class ParameterNode;

class NodeVisitor
{
public:
	virtual void Visit(std::shared_ptr<OperationNode> n) = 0;
	virtual void Visit(std::shared_ptr<InputInterfaceNode> n) = 0;
	virtual void Visit(std::shared_ptr<OutputInterfaceNode> n) = 0;
	virtual void Visit(std::shared_ptr<ParameterNode> n) = 0;
};
}  // namespace selector
