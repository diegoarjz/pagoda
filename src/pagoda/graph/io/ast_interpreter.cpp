#include "ast_interpreter.h"

#include "graph_definition_node.h"
#include "named_argument.h"
#include "node_definition_node.h"
#include "node_link_definition.h"
#include "node_link_node.h"

#include <pagoda/common/exception/exception.h>

#include <pagoda/dynamic/binding/cast_to.h>
#include <pagoda/dynamic/expression.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/integer_value.h>
#include <pagoda/dynamic/string_value.h>

#include <pagoda/graph/construction_argument_callback.h>
#include <pagoda/graph/construction_argument_not_found.h>
#include <pagoda/graph/execution_argument_callback.h>
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

struct ConstructionArgumentSetter : public ConstructionArgumentCallback
{
	ConstructionArgumentSetter(std::string &nodeName, std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &args)
	  : m_nodeName{nodeName}, m_args{args}
	{
	}

	void StringArgument(const char *const name, std::string &arg, const char *const label)
	{
		std::string n{name};
		if (m_args.find(n) == m_args.end()) {
			throw ConstructionArgumentNotFound(m_nodeName, name);
		}

		arg = dynamic::get_value_as<std::string>(*m_args[n]);
	}

	std::string &m_nodeName;
	std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &m_args;
};

class ExecutionArgumentSetter : public ExecutionArgumentCallback
{
	public:
	ExecutionArgumentSetter(std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &args) : m_args{args} {}

	DynamicValueBasePtr StringArgument(const char *const name, const char *const label,
	                                   const std::string &defaultValue = "")
	{
		if (auto value = GetArgument(name)) {
			return value;
		}
		return std::make_shared<String>(defaultValue);
	}

	DynamicValueBasePtr FloatArgument(const char *const name, const char *const label, float defaultValue = 0.0f)
	{
		if (auto value = GetArgument(name)) {
			return value;
		}
		return std::make_shared<FloatValue>(defaultValue);
	}

	DynamicValueBasePtr IntegerArgument(const char *const name, const char *const label, int defaultValue = 0)
	{
		if (auto value = GetArgument(name)) {
			return value;
		}
		return std::make_shared<Integer>(defaultValue);
	}

	DynamicValueBasePtr BooleanArgument(const char *const name, const char *const label, bool defaultValue = false)
	{
		if (auto value = GetArgument(name)) {
			return value;
		}
		return std::make_shared<Boolean>(defaultValue);
	}

	DynamicValueBasePtr PlaneArgument(const char *const name, const char *const label,
	                                  math::Plane<float> defaultValue = math::Plane<float>{})
	{
		if (auto value = GetArgument(name)) {
			return value;
		}
		return std::make_shared<DynamicPlane>(defaultValue);
	}

	private:
	dynamic::DynamicValueBasePtr GetArgument(const char *const name)
	{
		if (m_args.find(name) == m_args.end()) {
			return nullptr;
		}
		auto v = m_args[name];
		return m_args[name];
	}

	std::unordered_map<std::string, DynamicValueBasePtr> &m_args;
};

void AstInterpreter::Visit(NodeDefinitionNode *nodeDefinition)
{
	m_currentNamedParameters.clear();
	for (const auto &namedArgument : nodeDefinition->GetConstructionArguments()) {
		namedArgument->AcceptVisitor(this);
	}

	auto nodeName = nodeDefinition->GetNodeName();

	ConstructionArgumentSetter setter(nodeName, m_currentNamedParameters);
	nodeName = m_graph->CreateNode(nodeDefinition->GetNodeType(), nodeName);
	m_graph->SetNodeConstructionParameters(nodeName, &setter);

	m_currentNamedParameters.clear();
	for (const auto &namedArgument : nodeDefinition->GetExecutionArguments()) {
		namedArgument->AcceptVisitor(this);
	}
	ExecutionArgumentSetter executionSetter{m_currentNamedParameters};
	m_graph->SetNodeExecutionParameters(nodeName, &executionSetter);
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
