#ifndef SELECTOR_PROCEDURAL_GRAPH_XML_READER_PLUGIN_H_
#define SELECTOR_PROCEDURAL_GRAPH_XML_READER_PLUGIN_H_

#include <memory>

namespace selector
{
class Node;

class XMLReaderPlugin
{
public:
	XMLReaderPlugin() {}
	virtual const char *const PluginName() = 0;
	virtual void ParseNodeData(std::shared_ptr<Node> node, const char *valueName, const char *value) = 0;
};

}  // namespace selector
#endif
