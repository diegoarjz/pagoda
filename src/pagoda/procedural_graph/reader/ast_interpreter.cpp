#include "ast_interpreter.h"

#include "graph_definition_node.h"
#include "named_argument.h"
#include "node_definition_node.h"
#include "node_link_node.h"

#include <pagoda/common/exception/exception.h>
#include <pagoda/dynamic_value/expression.h>
#include <pagoda/dynamic_value/float_value.h>
#include <pagoda/dynamic_value/integer_value.h>
#include <pagoda/dynamic_value/string_value.h>
#include <pagoda/procedural_graph/graph.h>
#include <pagoda/procedural_graph/node.h>

namespace pagoda
{
AstInterpreter::AstInterpreter(GraphPtr graph) : m_graph(graph) {}

void AstInterpreter::Visit(GraphDefinitionNode *graphDefinition)
{
	for (const auto &statement : *graphDefinition)
	{
		statement->AcceptVisitor(this);
	}
}

void AstInterpreter::Visit(NamedArgument *namedArgument)
{
	DynamicValueBasePtr param;
	switch (namedArgument->GetArgumentType())
	{
		case NamedArgument::ArgumentType::String:
		{
			param = std::make_shared<String>(namedArgument->GetArgumentValue());
			break;
		}
		case NamedArgument::ArgumentType::Float:
		{
			param =
			    std::make_shared<FloatValue>(static_cast<float>(std::atof(namedArgument->GetArgumentValue().c_str())));
			break;
		}
		case NamedArgument::ArgumentType::Integer:
		{
			param = std::make_shared<Integer>(static_cast<int>(std::atoi(namedArgument->GetArgumentValue().c_str())));
			break;
		}
		case NamedArgument::ArgumentType::Expression:
		{
			param = Expression::CreateExpression(namedArgument->GetArgumentValue());
			break;
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

	auto node = m_graph->CreateNode(nodeDefinition->GetNodeType());
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
			throw common::exception::Exception("Node not found while linking '" + (*prevNodeName) + "' to '" +
			                                   (*currentNodeName) + "'");
		}

		m_graph->CreateEdge(prevNode->second, currNode->second);

		++prevNodeName;
		++currentNodeName;
	}
}

const std::unordered_map<std::string, NodePtr> &AstInterpreter::GetNodeTable() const { return m_nodeTable; }

const std::unordered_map<std::string, DynamicValueBasePtr> &AstInterpreter::GetCurrentNamedArguments() const
{
	return m_currentNamedParameters;
}

}  // namespace pagoda

