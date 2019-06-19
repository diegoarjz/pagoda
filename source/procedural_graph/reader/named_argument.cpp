#include "named_argument.h"

namespace selector
{
NamedArgument::NamedArgument(const Offset_t &startOffset, const Offset_t &endOffset, const std::string &name, const ArgumentType_t type, const std::string &value)
	: AstNode(startOffset, endOffset),
	  m_name(name),
	  m_argumentType(type),
	  m_argumentValue(value)
{
}

NamedArgument::NamedArgument(const Range_t &range, const std::string &name, const ArgumentType_t type, const std::string &value)
	: AstNode(Range),
	  m_name(name),
	  m_argumentType(type),
	  m_argumentValue(value)
{
}
    
NamedArgument::~NamedArgument()
{
}

const std::string& NamedArgument::GetName() const
{
	return m_name;
}

std::string& NamedArgument::GetName()
{
	return m_name;
}
	
void NamedArgument::SetName(const std::string& name)
{
	m_name = name;
}

const ArgumentType& NamedArgument::GetArgumentType() const
{
	return m_argumentType;
}

ArgumentType& NamedArgument::GetArgumentType()
{
	return m_argumentType;
}

void NamedArgument::SetArgumentType(const ArgumentType& type)
{
	m_argumentType = type;
}

const std::string& NamedArgument::GetArgumentValue() const
{
	return m_argumentValue;
}

std::string& NamedArgument::GetArgumentValue()
{
	return m_argumentValue;
}

void NamedArgument::SetArgumentValue(const std::string &argumentValue)
{
	m_argumentValue = argumentValue;
}
}