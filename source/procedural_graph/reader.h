#ifndef SELECTOR_PROCEDURAL_GRAPH_READER_H_
#define SELECTOR_PROCEDURAL_GRAPH_READER_H_

#include "graph.h"
#include "node.h"
#include "xml_parse_result.h"

#include <string>
#include <unordered_map>

namespace selector
{
class XMLReaderPlugin;

class XMLReader
{
public:
	XMLReader();
	~XMLReader();
	ParseResult Read(const std::string &xml);
	std::shared_ptr<Graph> GetGraph() const;

	std::shared_ptr<Node> CreateNode(NodeType type, const char *name, uint32_t id);
	void Link(NodePtr from, NodePtr to);

	bool RegisterPlugin(std::shared_ptr<XMLReaderPlugin> plugin);
	void PluginNodeData(const char *pluginName, std::shared_ptr<Node> node, const char *valueName, const char *value);

private:
	void SetParseResult(const ParseResult::Status &status, uint32_t offset);
	std::shared_ptr<XMLReaderPlugin> GetPlugin(const char *name);

	ParseResult m_currentParseResult;
	std::shared_ptr<Graph> m_graph;
	std::unordered_map<std::string, std::shared_ptr<XMLReaderPlugin>> m_plugins;
};  // class XMLReader
}  // namespace selector

#endif
