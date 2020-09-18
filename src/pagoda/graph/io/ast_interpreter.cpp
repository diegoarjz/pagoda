#include "ast_interpreter.h"

#include "graph_definition_node.h"
#include "named_argument.h"
#include "node_definition_node.h"
#include "node_link_definition.h"
#include "node_link_node.h"

#include <pagoda/common/exception/exception.h>

#include <pagoda/dynamic/expression.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/integer_value.h>
#include <pagoda/dynamic/string_value.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/output_interface_node.h>

#include <memory>

using namespace pagoda::dynamic;

namespace pagoda::graph::io
{
AstInterpreter::AstInterpreter(GraphPtr graph) : m_graph(graph) {}
AstInterpreter::~AstInterpreter() {}

void AstInterpreter::Visit(GraphDefinitionNode *graphDefinition)
{
	for (const auto &statement : *graphDefinition) {
		statement->AcceptVisitor(this);
	}
}

void AstInterpreter::Visit(NamedArgument *namedArgument)
{
	DynamicValueBasePtr param;
	switch (namedArgument->GetArgumentType()) {
		case NamedArgument::ArgumentType::String: {
			param = std::make_shared<String>(namedArgument->GetArgumentValue());
			break;
		}
		case NamedArgument::ArgumentType::Float: {
			param = std::make_shared<FloatValue>(static_cast<float>(std::atof(namedArgument->GetArgumentValue().c_str())));
			break;
		}
		case NamedArgument::ArgumentType::Integer: {
			param = std::make_shared<Integer>(static_cast<int>(std::atoi(namedArgument->GetArgumentValue().c_str())));
			break;
		}
		case NamedArgument::ArgumentType::Expression: {
			param = Expression::CreateExpression(namedArgument->GetArgumentValue());
			break;
		}
	}
	m_currentNamedParameters[namedArgument->GetName()] = param;
}

void AstInterpreter::Visit(NodeDefinitionNode *nodeDefinition)
{
	m_currentNamedParameters.clear();
	for (const auto &namedArgument : nodeDefinition->GetConstructionArguments()) {
		namedArgument->AcceptVisitor(this);
	}

	auto nodeName = nodeDefinition->GetNodeName();
	nodeName = m_graph->CreateNode(nodeDefinition->GetNodeType(), nodeName);
	m_graph->SetNodeConstructionParameters(nodeName, m_currentNamedParameters);

	m_currentNamedParameters.clear();
	for (const auto &namedArgument : nodeDefinition->GetExecutionArguments()) {
		namedArgument->AcceptVisitor(this);
	}
	m_graph->SetNodeExecutionParameters(nodeName, m_currentNamedParameters);
}

void AstInterpreter::Visit(NodeLinkNode *nodeLink)
{
	auto end = nodeLink->end();
	auto prevNodeDefinition = nodeLink->begin();
	auto currentNodeDefinition = std::next(prevNodeDefinition);

	while (currentNodeDefinition != end) {
		(*prevNodeDefinition)->AcceptVisitor(this);
		(*currentNodeDefinition)->AcceptVisitor(this);

		m_graph->CreateEdge((*prevNodeDefinition)->GetOutNode(), (*currentNodeDefinition)->GetInNode());

		++prevNodeDefinition;
		++currentNodeDefinition;
	}
}

void AstInterpreter::Visit(NodeLinkDefinition *nodeLinkDefinition)
{
	auto inInterface = nodeLinkDefinition->GetInputInterface();
	auto node = nodeLinkDefinition->GetNodeName();
	auto outInterface = nodeLinkDefinition->GetOutputInterface();

	if (!inInterface.empty()) {
		auto inputInterfaceName = nodeLinkDefinition->GetInNode();
		if (m_graph->GetNode(inputInterfaceName) == nullptr) {
			m_graph->CreateNode<InputInterfaceNode>(inputInterfaceName);
			auto inInterfaceNode = std::dynamic_pointer_cast<InputInterfaceNode>(m_graph->GetNode(inputInterfaceName));
			inInterfaceNode->SetInterfaceName(inInterface);
		}
		m_graph->CreateEdge(inputInterfaceName, node);
	}
	if (!outInterface.empty()) {
		auto outputInterfaceName = nodeLinkDefinition->GetOutNode();
		if (m_graph->GetNode(outputInterfaceName) == nullptr) {
			m_graph->CreateNode<OutputInterfaceNode>(outputInterfaceName);
			auto outInterfaceNode = std::dynamic_pointer_cast<OutputInterfaceNode>(m_graph->GetNode(outputInterfaceName));
			outInterfaceNode->SetInterfaceName(outInterface);
		}
		m_graph->CreateEdge(node, outputInterfaceName);
	}
}

const std::unordered_map<std::string, DynamicValueBasePtr> &AstInterpreter::GetCurrentNamedArguments() const
{
	return m_currentNamedParameters;
}

}  // namespace pagoda::graph::io
