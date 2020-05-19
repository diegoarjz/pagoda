#include "named_argument.h"

#include "ast_node_visitor.h"

namespace pagoda::graph::io
{
NamedArgument::NamedArgument(const std::string& name, const ArgumentType type, const std::string& value)
    : m_name(name), m_argumentType(type), m_argumentValue(value)
{
}

NamedArgument::NamedArgument(const AstNode::Offset_t& startOffset, const AstNode::Offset_t& endOffset,
                             const std::string& name, const NamedArgument::ArgumentType type, const std::string& value)
    : AstNode(startOffset, endOffset), m_name(name), m_argumentType(type), m_argumentValue(value)
{
}

NamedArgument::NamedArgument(const AstNode::NodeRange_t& range, const std::string& name,
                             const NamedArgument::ArgumentType type, const std::string& value)
    : AstNode(range), m_name(name), m_argumentType(type), m_argumentValue(value)
{
}

NamedArgument::~NamedArgument() {}

const std::string& NamedArgument::GetName() const { return m_name; }

std::string& NamedArgument::GetName() { return m_name; }

void NamedArgument::SetName(const std::string& name) { m_name = name; }

const NamedArgument::ArgumentType& NamedArgument::GetArgumentType() const { return m_argumentType; }

NamedArgument::ArgumentType& NamedArgument::GetArgumentType() { return m_argumentType; }

void NamedArgument::SetArgumentType(const NamedArgument::ArgumentType& type) { m_argumentType = type; }

const std::string& NamedArgument::GetArgumentValue() const { return m_argumentValue; }

std::string& NamedArgument::GetArgumentValue() { return m_argumentValue; }

void NamedArgument::SetArgumentValue(const std::string& argumentValue) { m_argumentValue = argumentValue; }

void NamedArgument::AcceptVisitor(AstNodeVisitor* visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<NamedArgument>(shared_from_this()).get());
}
}  // namespace pagoda::graph::io
