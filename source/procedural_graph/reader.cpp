#include "reader.h"

#include "common/assertions.h"
#include "input_interface_node.h"
#include "operation_node.h"
#include "output_interface_node.h"
#include "parameter/context.h"
#include "parameter/expression.h"
#include "xml_reader_plugin.h"
#include "xml_reader_versions.h"

#include "common/pugixml.hpp"

#include <cstring>
#include <iostream>
#include <unordered_map>

namespace selector
{
ParseResult::Status convert_status_code(const pugi::xml_parse_status &status)
{
	switch (status)
	{
		case pugi::xml_parse_status::status_ok:
			return ParseResult::Status::Ok;
		case pugi::xml_parse_status::status_file_not_found:
			return ParseResult::Status::FileNotFound;
		case pugi::xml_parse_status::status_io_error:
			return ParseResult::Status::IOError;
		case pugi::xml_parse_status::status_out_of_memory:
			return ParseResult::Status::OutOfMemory;
		case pugi::xml_parse_status::status_internal_error:
			return ParseResult::Status::InternalError;
		case pugi::xml_parse_status::status_unrecognized_tag:
			return ParseResult::Status::UnrecognizedTag;
		case pugi::xml_parse_status::status_bad_pi:
		case pugi::xml_parse_status::status_bad_comment:
		case pugi::xml_parse_status::status_bad_cdata:
		case pugi::xml_parse_status::status_bad_doctype:
		case pugi::xml_parse_status::status_bad_pcdata:
		case pugi::xml_parse_status::status_bad_start_element:
		case pugi::xml_parse_status::status_bad_attribute:
		case pugi::xml_parse_status::status_bad_end_element:
			return ParseResult::Status::BadElement;
		case pugi::xml_parse_status::status_end_element_mismatch:
			return ParseResult::Status::EndElementMismatch;
		case pugi::xml_parse_status::status_no_document_element:
			return ParseResult::Status::NoDocumentElement;
		default:
			DBG_ASSERT_MSG(false, "Should not be able to reach this");
	}
	return ParseResult::Status::UnknownError;
}

XMLReader::XMLReader() : m_graph(std::make_shared<Graph>()) {}
XMLReader::~XMLReader() {}
std::shared_ptr<Graph> XMLReader::GetGraph() const { return m_graph; }

ParseResult XMLReader::Read(const std::string &xml)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(xml.c_str());

	if (!result)
	{
		SetParseResult(convert_status_code(result.status), result.offset);
		return m_currentParseResult;
	}

	pugi::xml_node root = doc.child(xml_common::root_node);
	if (!root)
	{
		SetParseResult(ParseResult::Status::GraphElementNotFound, 0);
		return m_currentParseResult;
	}

	auto reader = xml_common::create_reader(doc, this);
	return reader->Read(doc);
}

std::shared_ptr<Node> XMLReader::CreateNode(const char *type, const char *name, uint32_t id)
{
	NodePtr node = nullptr;
	if (std::strcmp(type, "InputInterface") == 0)
	{
		node = m_graph->CreateNode<InputInterfaceNode>();
	}
	else if (std::strcmp(type, "OutputInterface") == 0)
	{
		node = m_graph->CreateNode<OutputInterfaceNode>();
	}
	else if (std::strcmp(type, "Operation") == 0)
	{
		node = m_graph->CreateNode<OperationNode>();
	}

	DBG_ASSERT(node != nullptr);

	node->SetName(name);
	return node;
}

void XMLReader::Link(NodePtr from, NodePtr to)
{
	// TODO: Error checking
	m_graph->CreateEdge(from, to);
}

void XMLReader::SetParseResult(const ParseResult::Status &status, uint32_t offset)
{
	m_currentParseResult.status = status;
	m_currentParseResult.offset = offset;
}

bool XMLReader::RegisterPlugin(std::shared_ptr<XMLReaderPlugin> plugin)
{
	if (m_plugins.find(plugin->PluginName()) != m_plugins.end())
	{
		return false;
	}

	m_plugins[plugin->PluginName()] = plugin;

	return true;
}

std::shared_ptr<XMLReaderPlugin> XMLReader::GetPlugin(const char *name)
{
	if (m_plugins.find(name) == m_plugins.end())
	{
		return nullptr;
	}

	return m_plugins[name];
}

void XMLReader::PluginNodeData(const char *pluginName, std::shared_ptr<Node> node, const char *valueName,
                               const char *value)
{
	std::shared_ptr<XMLReaderPlugin> plugin = GetPlugin(pluginName);

	if (plugin == nullptr)
	{
		return;
	}

	plugin->ParseNodeData(node, valueName, value);
}

}  // namespace selector
