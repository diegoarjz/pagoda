#include "ast_interpreter.h"

#include "graph_definition_node.h"
#include "named_argument.h"
#include "node_definition_node.h"
#include "node_link_definition.h"
#include "node_link_node.h"

#include <pagoda/common/exception/exception.h>
#include <pagoda/common/exception/unimplemented.h>

#include <pagoda/dynamic/binding/cast_to.h>
#include <pagoda/dynamic/expression.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/integer_value.h>
#include <pagoda/dynamic/string_value.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/output_interface_node.h>

#include <pagoda/objects/parameter_callback.h>
#include <pagoda/objects/parameter_creator.h>

#include <memory>

using namespace pagoda::dynamic;

namespace pagoda::graph::io
{
AstInterpreter::AstInterpreter(GraphPtr graph) : m_graph(graph)
{
}
AstInterpreter::~AstInterpreter()
{
}

void AstInterpreter::Visit(GraphDefinitionNode *graphDefinition)
{
	for (const auto &statement : *graphDefinition) {
		statement->AcceptVisitor(this);
	}
}

void AstInterpreter::Visit(NamedArgument *namedArgument)
{
	auto par = m_currentNode->GetParameter(namedArgument->GetName());
	if (par == nullptr) {
		LOG_WARNING("Parameter " << namedArgument->GetName()
		                         << " doesn't exist in node "
		                         << m_currentNode->GetName());
		return;
	}
	if (namedArgument->GetArgumentType() ==
	    NamedArgument::ArgumentType::Expression) {
		par->SetExpression(namedArgument->GetArgumentValue());
	} else {
		par->FromString(namedArgument->GetArgumentValue());
	}
}

void AstInterpreter::Visit(NodeDefinitionNode *nodeDefinition)
{
	const std::string nodeName = m_graph->CreateNode(
	  nodeDefinition->GetNodeType(), nodeDefinition->GetNodeType());
	m_currentNode = m_graph->GetNode(nodeName);

	const auto &executionArgs = nodeDefinition->GetExecutionArguments();
	for (const auto &namedArgument : executionArgs) {
		namedArgument->AcceptVisitor(this);
	}
}

void AstInterpreter::Visit(NodeLinkNode *nodeLink)
{
	auto end = nodeLink->end();
	auto prevNodeDefinition = nodeLink->begin();
	auto currentNodeDefinition = std::next(prevNodeDefinition);

	while (currentNodeDefinition != end) {
		(*prevNodeDefinition)->AcceptVisitor(this);
		(*currentNodeDefinition)->AcceptVisitor(this);

		m_graph->CreateEdge((*prevNodeDefinition)->GetOutNode(),
		                    (*currentNodeDefinition)->GetInNode());

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
			auto inInterfaceNode = std::dynamic_pointer_cast<InputInterfaceNode>(
			  m_graph->GetNode(inputInterfaceName));
			inInterfaceNode->SetInterfaceName(inInterface);
		}
		m_graph->CreateEdge(inputInterfaceName, node);
	}
	if (!outInterface.empty()) {
		auto outputInterfaceName = nodeLinkDefinition->GetOutNode();
		if (m_graph->GetNode(outputInterfaceName) == nullptr) {
			m_graph->CreateNode<OutputInterfaceNode>(outputInterfaceName);
			auto outInterfaceNode = std::dynamic_pointer_cast<OutputInterfaceNode>(
			  m_graph->GetNode(outputInterfaceName));
			outInterfaceNode->SetInterfaceName(outInterface);
		}
		m_graph->CreateEdge(node, outputInterfaceName);
	}
}

}  // namespace pagoda::graph::io
