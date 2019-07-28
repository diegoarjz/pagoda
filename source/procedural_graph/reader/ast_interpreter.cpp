#include "ast_interpreter.h"

#include "graph_definition_node.h"
#include "named_argument.h"
#include "node_definition_node.h"
#include "node_link_node.h"

#include "parameter/context.h"
#include "procedural_graph/graph.h"
#include "procedural_graph/node.h"

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
			param = static_cast<float>(std::atof(namedArgument->GetArgumentValue().c_str()));
			break;
		}
		case NamedArgument::ArgumentType::Integer:
		{
			param = static_cast<int>(std::atoi(namedArgument->GetArgumentValue().c_str()));
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
	m_currentNamedParameters[namedArgument->GetName()] = param;
}

void AstInterpreter::Visit(NodeDefinitionNode *nodeDefinition)
{
	m_currentNamedParameters.clear();
	for (const auto &namedArgument : nodeDefinition->GetConstructionArguments())
	{
		namedArgument->AcceptVisitor(this);
	}

	auto node = Node::Create(nodeDefinition->GetNodeType());
	node->SetParameterContext(std::make_shared<Context>(nodeDefinition->GetNodeName()));
	node->SetId(m_nextNodeId++);
	node->SetName(nodeDefinition->GetNodeName());
	node->SetConstructionArguments(m_currentNamedParameters);

	m_currentNamedParameters.clear();
	for (const auto &namedArgument : nodeDefinition->GetExecutionArguments())
	{
		namedArgument->AcceptVisitor(this);
	}
	node->SetExecutionArguments(m_currentNamedParameters);

	m_graph->AddNode(node);
	m_nodeTable[nodeDefinition->GetNodeName()] = node;
}

void AstInterpreter::Visit(NodeLinkNode *nodeLink)
{
	auto end = nodeLink->end();
	auto prevNodeName = nodeLink->begin();
	auto currentNodeName = std::next(prevNodeName);

	while (currentNodeName != end)
	{
		auto prevNode = m_nodeTable.find(*prevNodeName);
		auto currNode = m_nodeTable.find(*currentNodeName);

		if (prevNode == std::end(m_nodeTable) || currNode == std::end(m_nodeTable))
		{
			// THROW
		}

		m_graph->CreateEdge(prevNode->second, currNode->second);

		++prevNodeName;
		++currentNodeName;
	}
}

const std::unordered_map<std::string, NodePtr> &AstInterpreter::GetNodeTable() const { return m_nodeTable; }

const std::unordered_map<std::string, Parameter> &AstInterpreter::GetCurrentNamedArguments() const
{
	return m_currentNamedParameters;
}

}  // namespace selector

