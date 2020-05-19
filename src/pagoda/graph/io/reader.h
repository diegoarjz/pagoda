#ifndef PAGODA_PROCEDURAL_GRAPH_READER_H_
#define PAGODA_PROCEDURAL_GRAPH_READER_H_

#include "parse_result.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

#include <string>
#include <unordered_map>

namespace pagoda::graph
{
class NodeFactory;
using NodeFactoryPtr = std::shared_ptr<NodeFactory>;
}  // namespace pagoda::graph

namespace pagoda::graph::io
{
class GraphReader
{
public:
	GraphReader(NodeFactoryPtr nodeFactory);
	~GraphReader();

	GraphPtr Read(const std::string &str);
	const ParseResult &GetParseResult() const;

private:
	struct Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pagoda::graph::io

#endif
