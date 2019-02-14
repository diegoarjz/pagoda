#ifndef SELECTOR_PROCEDURAL_GRAPH_XML_READER_VERSIONS_H_
#define SELECTOR_PROCEDURAL_GRAPH_XML_READER_VERSIONS_H_

#include "common/const_str.h"
#include "procedural_objects/procedural_operation.h"
#include "xml_parse_result.h"

#include "common/pugixml.hpp"

#include <unordered_map>

namespace selector
{
class XMLReader;
class Node;

namespace xml_common
{
enum class XMLVersion
{
	V01,
	UnknownVersion
};

// clang-format off
constexpr auto xpath_separator = literal("/");
constexpr auto xpath_root = literal("//");

constexpr auto root_node = literal("graph");

namespace header
{
    constexpr auto tag = literal("header");
    constexpr auto version = literal("version");
}
// clang-format on

class XMLReaderVersioned
{
public:
	XMLReaderVersioned(XMLReader *reader);
    virtual ~XMLReaderVersioned() {}
	virtual ParseResult Read(const pugi::xml_document &doc) = 0;

	void SetParseResult(const ParseResult::Status &status, uint32_t offset);

protected:
	XMLReader *reader;
	ParseResult current_parse_result;
};  // class XMLReaderVersioned

XMLVersion get_version(const pugi::xml_document &doc);
std::unique_ptr<XMLReaderVersioned> create_reader(const pugi::xml_document &doc, XMLReader *reader);
}  // namespace xml_common

namespace xml_v01
{
// clang-format off

constexpr auto version = literal("0.1");

namespace node
{
    constexpr auto collection_tag = literal("nodes");
    constexpr auto tag = literal("node");

    constexpr auto type = literal("type");
    constexpr auto id = literal("id");
    constexpr auto name = literal("name");

    namespace types
    {
        constexpr auto operation = literal("Operation");
        constexpr auto input_interface = literal("InputInterface");
        constexpr auto output_interface = literal("OutputInterface");
    }  // namespace types

    namespace custom_data
    {
        constexpr auto tag = literal("customData");
        constexpr auto data_tag = literal("data");
        constexpr auto namespace_attr = literal("namespace");
        constexpr auto name = literal("name");
        constexpr auto value = literal("value");
    }
}  // namespace node

namespace link
{
    constexpr auto collection_tag = literal("links");
    constexpr auto tag = literal("link");

    constexpr auto from = literal("from");
    constexpr auto to = literal("to");
}  // namespace link

namespace operation
{
    constexpr auto tag = literal("operation");
    constexpr auto type = literal("type");

    constexpr auto extrude = literal("Extrude");
    constexpr auto createRect = literal("CreateRect");
    constexpr auto triangulate = literal("Triangulate");
}  // namespace operation

namespace parameter
{
    constexpr auto collection_tag = literal("parameters");
    constexpr auto tag = literal("parameter");

    constexpr auto name = literal("name");
    constexpr auto type = literal("type");
    constexpr auto value = literal("value");
    constexpr auto expression = literal("expression");
}  // namespace parameter
// clang-format on

class XMLReaderV01 : public xml_common::XMLReaderVersioned
{
public:
	XMLReaderV01(XMLReader *reader);
	ParseResult Read(const pugi::xml_document &doc) final;

private:
	std::shared_ptr<Node> CreateOperationNode(std::shared_ptr<Node> operation_node, const pugi::xml_node &node);
	std::shared_ptr<Node> CreateInputInterfaceNode(std::shared_ptr<Node> node, const pugi::xml_node &xml_node);
	std::shared_ptr<Node> CreateOutputInterfaceNode(std::shared_ptr<Node> node, const pugi::xml_node &xml_node);
	std::shared_ptr<ParameterBase> CreateParameter(std::shared_ptr<Context> context, const char *name, const char *type,
	                                               const char *value, bool is_expression);
	std::shared_ptr<ProceduralOperation> CreateOperation(const char *operationName);
	std::unordered_map<uint32_t, std::shared_ptr<Node>> nodes_map;
};  // class XMLReaderV01
}  // namespace xml_v01

}  // namespace selector

#endif
