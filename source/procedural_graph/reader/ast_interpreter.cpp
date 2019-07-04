#include "ast_interpreter.h"

#include "procedural_graph/graph.h"

namespace selector
{

AstInterpreter::AstInterpreter(GraphPtr graph) : m_graph(graph), m_nextNodeId(0) {}

void AstInterpreter::Visit(GraphDefinitionNode *graphDefinition)
{
	for (const auto &statement : *graphDefinition)
	{
		statement->AcceptVisitor(this);
	}
}

void AstInterpreter::Visit(NamedArgument *namedArgument)
{
	Parameter param;
	switch (namedArgument->GetArgumentType())
	{
		case NamedArgument::ArgumentType::String:
		{
			param = namedArgument->GetArgumentValue();
			break;
		}
		case NamedArgument::ArgumentType::Float:
		{
			param = std::atof(namedArgument->GetArgumentValue());
			break;
		}
		case NamedArgument::ArgumentType::Expression:
		{
			param = Expression::CreateExpression(namedArgument->GetArgumentValue());
			break;
		}
		default:
		{
			// THROW
		}
	}
	m_namedArguments[namedArgument->GetName()] = param;
}

void AstInterpreter::Visit(NodeDefinitionNode *nodeDefinition)
{
	m_namedArguments.clear();
	for (const auto& namedArgument : nodeDefinition->GetConstructionArguments())
	{
		namedArgument->AcceptVisitor(this);
	}
	
	auto node = Node::Create(nodeDefinition->GetNodeType());
	node->SetNodeId(m_nextNodeId++);
	node->SetName(nodeDefinition->GetNodeName());
	node->SetConstructionArguments(m_namedArguments);
	
	m_namedArguments.clear();
	for (const auto& namedArgument : nodeDefinition->GetExecutionArguments())
	{
		namedArgument->AcceptVisitor(this);
	}
	node->SetExecutionArguments(m_namedArguments);
	
	m_graph->AddNode(node);
}

void AstInterpreter::Visit(NodeLinkNode *nodeLink)
{
	auto end = nodeLink->end();
	auto prevNodeName = nodeLink->begin();
	auto currentNodeName = std::next(prevNode);
	
	while (currentNodeName != end)
	{
		auto prevNode = m_nodeTable.find(*prevNode);
		auto currNode = m_nodeTable.find(*currentNode);
		
		if (prevNode == std::end(m_nodeTable) || currentNode == std::end(m_nodeTable))
		{
			// THROW
		}
		
		m_graph->CreateEdge(*prevNode, *currentNode);
		
		++prevNodeName;
		++currentNodeName;
	}
}

const std::unordered_map<std::string, NodePtr>& AstInterpreter::GetNodeTable() const
{
	return m_nodeTable;
}

const std::unordered_map<std::string, Parameter>& AstInterpreter::GetCurrentNamedArguments() const
{
	return m_currentNamedParameters;
}

}
	