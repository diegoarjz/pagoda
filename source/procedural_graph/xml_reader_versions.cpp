#include "xml_reader_versions.h"

#include "node.h"
#include "operation_node.h"
#include "reader.h"

#include "common/utils.h"

#include "parameter/context.h"
#include "parameter/float_parameter.h"
#include "parameter/parameter.h"

#include "procedural_objects/create_rect.h"
#include "procedural_objects/extrude_geometry.h"
#include "procedural_objects/procedural_operation_factory.h"

#include <iostream>

namespace selector
{
namespace xml_common
{
XMLVersion get_version(const pugi::xml_document& doc)
{
	pugi::xml_node root = doc.child(root_node);

	pugi::xml_node header_node = root.child(header::tag);
	if (!header_node)
	{
		return XMLVersion::UnknownVersion;
	}

	pugi::xml_node version_node = header_node.child(header::version);
	if (!version_node)
	{
		return XMLVersion::UnknownVersion;
	}

	const char* version_string = version_node.text().as_string();

	if (std::strcmp(version_string, xml_v01::version) == 0)
	{
		return XMLVersion::V01;
	}

	return XMLVersion::UnknownVersion;
}

std::unique_ptr<XMLReaderVersioned> create_reader(const pugi::xml_document& doc, XMLReader* reader)
{
	XMLVersion version = get_version(doc);

	switch (version)
	{
		case XMLVersion::V01:
			return std::make_unique<xml_v01::XMLReaderV01>(reader);
		default:
			return nullptr;
	}
}

XMLReaderVersioned::XMLReaderVersioned(XMLReader* reader) : reader(reader) {}

void XMLReaderVersioned::SetParseResult(const ParseResult::Status& status, uint32_t offset)
{
	current_parse_result.status = status;
	current_parse_result.offset = offset;
}
}  // namespace xml_common

namespace xml_v01
{
ProceduralOperationType convert_operation_type(const char* type)
{
	if (std::strcmp(type, operation::extrude) == 0)
	{
		return ProceduralOperationType::Extrude;
	}

	if (std::strcmp(type, operation::createRect) == 0)
	{
		return ProceduralOperationType::CreateRect;
	}

	if (std::strcmp(type, operation::triangulate) == 0)
	{
		return ProceduralOperationType::Triangulate;
	}

	return ProceduralOperationType::Max;
}

XMLReaderV01::XMLReaderV01(XMLReader* reader) : XMLReaderVersioned(reader) {}

ParseResult XMLReaderV01::Read(const pugi::xml_document& doc)
{
	pugi::xml_node root = doc.child(xml_common::root_node);
	pugi::xml_node nodes = root.child(xml_v01::node::collection_tag);
	pugi::xml_node links = root.child(xml_v01::link::collection_tag);

	for (auto node = nodes.child(xml_v01::node::tag); node; node = node.next_sibling(xml_v01::node::tag))
	{
		uint32_t node_id = node.attribute(xml_v01::node::id).as_uint();
		const char* node_name = node.attribute(xml_v01::node::name).as_string();
		const char* node_type = node.attribute(xml_v01::node::type).as_string();

		NodePtr nodePtr = reader->CreateNode(node_type, node_name, node_id);
		auto inserted = nodes_map.insert(std::make_pair(node_id, nodePtr));

		ParameterIdentifier identifier = ParameterIdentifier::CreateIdentifier(node_name).second;
		auto parameter_context = std::make_shared<Context>(identifier);
		nodePtr->SetParameterContext(parameter_context);

		if (!inserted.second)
		{
			ParseResult result;
			result.status = ParseResult::Status::DuplicatedNodeId;
			result.offset = 0;
			return result;
		}

		if (std::strcmp(node_type, "InputInterface") == 0)
		{
			CreateInputInterfaceNode(nodePtr, node);
		}
		else if (std::strcmp(node_type, "OutputInterface") == 0)
		{
			CreateOutputInterfaceNode(nodePtr, node);
		}
		else if (std::strcmp(node_type, "Operation") == 0)
		{
			CreateOperationNode(nodePtr, node);
		}
		else
		{
			ParseResult result;
			result.offset = 0;
			result.status = ParseResult::Status::UnknownNodeType;
			return result;
		}

		// Read node custom data
		pugi::xml_node custom_data_node = node.child(xml_v01::node::custom_data::tag);
		for (auto data = custom_data_node.child(xml_v01::node::custom_data::data_tag); data;
		     data = data.next_sibling(xml_v01::node::custom_data::data_tag))
		{
			const char* data_namespace = data.attribute(xml_v01::node::custom_data::namespace_attr).as_string();
			const char* name = data.attribute(xml_v01::node::custom_data::name).as_string();
			const char* value = data.attribute(xml_v01::node::custom_data::value).as_string();
			reader->PluginNodeData(data_namespace, nodePtr, name, value);
		}
	}

	for (auto link = links.child(xml_v01::link::tag); link; link = link.next_sibling(xml_v01::link::tag))
	{
		uint32_t from = link.attribute(xml_v01::link::from).as_uint();
		uint32_t to = link.attribute(xml_v01::link::to).as_uint();

		// TODO: Error checking
		reader->Link(nodes_map.at(from), nodes_map.at(to));
	}

	ParseResult result;
	result.offset = 0;
	result.status = ParseResult::Status::Ok;
	return result;
}

std::shared_ptr<ParameterBase> XMLReaderV01::CreateParameter(std::shared_ptr<Context> context, const char* name,
                                                             const char* type, const char* value, bool is_expression)
{
	if (std::strcmp(type, "float") == 0)
	{
		if (!is_expression)
		{
			if (!is_float(value))
			{
				return nullptr;
			}
			return context->CreateParameter<FloatParameter>(name, std::atof(value));
		}
		else
		{
			auto expression = std::make_shared<Expression>(value);
			auto param = context->CreateParameter<FloatParameter>(name, expression);

			return param;
		}
	}

	return nullptr;
}

NodePtr XMLReaderV01::CreateOperationNode(NodePtr node, const pugi::xml_node& xml_node)
{
	auto operationNode = std::dynamic_pointer_cast<OperationNode>(node);
	pugi::xml_node xml_operation_node = xml_node.child(operation::tag);
	const char* operation_type = xml_operation_node.attribute(operation::type).as_string();
	auto operation = ProceduralOperationFactory::Create(convert_operation_type(operation_type));
	operationNode->SetOperation(operation);

	pugi::xml_node operation_parameters = xml_operation_node.child(parameter::collection_tag);

	for (pugi::xml_node par = operation_parameters.child(parameter::tag); par; par = par.next_sibling(parameter::tag))
	{
		auto param_name = par.attribute(parameter::name);
		auto param_type = par.attribute(parameter::type);
		auto param_value = par.attribute(parameter::value);
		auto is_expression = par.attribute(parameter::expression);

		if (!param_name || !param_type || !param_value || !is_expression)
		{
			SetParseResult(ParseResult::Status::InvalidParameter, par.offset_debug());
			return nullptr;
		}

		auto param = CreateParameter(node->GetParameterContext(), param_name.as_string(), param_type.as_string(),
		                             param_value.as_string(), is_expression.as_bool());

		if (!param)
		{
			SetParseResult(ParseResult::Status::InvalidParameter, par.offset_debug());
			return nullptr;
		}
	}

	return node;
}

std::shared_ptr<Node> XMLReaderV01::CreateInputInterfaceNode(std::shared_ptr<Node> node, const pugi::xml_node& xml_node)
{
	// TODO: Interface name
	return node;
}

std::shared_ptr<Node> XMLReaderV01::CreateOutputInterfaceNode(std::shared_ptr<Node> node,
                                                              const pugi::xml_node& xml_node)
{
	// TODO: Interface name
	return node;
}
}  // namespace xml_v01
}  // namespace selector
