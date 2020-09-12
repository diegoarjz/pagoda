#include "reader.h"

#include "parse_result.h"

#include "ast_interpreter.h"
#include "graph_definition_node.h"
#include "graph_reader_grammar.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/exception.h>
#include <pagoda/graph/node_factory.h>

#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace pagoda::graph::io
{
struct GraphReader::Impl
{
	Impl(NodeFactoryPtr nodeFactory) : m_nodeFactory(nodeFactory), m_currentParseResult({ParseResult::Status::Ok, 0}) {}

	GraphPtr Read(const std::string &str)
	{
		GraphPtr graph = std::make_shared<Graph>(m_nodeFactory);

		std::string::const_iterator begin = std::begin(str);
		std::string::const_iterator end = std::end(str);

		GraphReaderGrammar<std::string::const_iterator> grammar;
		GraphDefinitionNodePtr graph_def;

		bool result = boost::spirit::qi::phrase_parse(begin, end, grammar, boost::spirit::qi::space, graph_def);

		if (!result || begin != end) {
			throw common::exception::Exception("Syntax error while reading graph file. Starting in\n " +
			                                   std::string(begin, end));
			m_currentParseResult.status = ParseResult::Status::UnknownError;
			return nullptr;
		}

		AstInterpreter interpreter(graph);
		interpreter.Visit(graph_def.get());

		return graph;
	}
	const ParseResult &GetParseResult() const { return m_currentParseResult; }

	private:
	NodeFactoryPtr m_nodeFactory;
	ParseResult m_currentParseResult;
};

GraphReader::GraphReader(NodeFactoryPtr nodeFactory)
  : m_implementation(std::make_unique<GraphReader::Impl>(nodeFactory))
{
}
GraphReader::~GraphReader() {}
GraphPtr GraphReader::Read(const std::string &str) { return m_implementation->Read(str); }
const ParseResult &GraphReader::GetParseResult() const { return m_implementation->GetParseResult(); }

}  // namespace pagoda::graph::io
