#ifndef SELECTOR_PROCEDURAL_GRAPH_READER_H_
#define SELECTOR_PROCEDURAL_GRAPH_READER_H_

#include "graph.h"
#include "node.h"
#include "parse_result.h"

#include <string>
#include <unordered_map>

namespace selector
{
class GraphReader
{
public:
	GraphReader();
	~GraphReader();

	GraphPtr Read(const std::string &str);
	const ParseResult &GetParseResult() const;

private:
	struct Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace selector

#endif
