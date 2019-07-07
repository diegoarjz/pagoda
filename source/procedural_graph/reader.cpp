#include "reader.h"

#include "reader/ast_interpreter.h"
#include "reader/graph_definition_node.h"
#include "reader/graph_reader_grammar.h"

#include "common/assertions.h"
#include "common/logger.h"
#include "parse_result.h"

#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace selector
{
struct GraphReader::Impl
{
	Impl() : m_currentParseResult({ParseResult::Status::Ok, 0}) {}
	GraphPtr Read(const std::string &str)
	{
		GraphPtr graph = std::make_shared<Graph>();

		std::string::const_iterator begin = std::begin(str);
		std::string::const_iterator end = std::end(str);

		GraphReaderGrammar<std::string::const_iterator> grammar;
		GraphDefinitionNodePtr graph_def;

		bool result = boost::spirit::qi::phrase_parse(begin, end, grammar, boost::spirit::qi::space, graph_def);

		if (!result || begin != end)
		{
			std::cout << " found: " << std::string(begin, end);
			m_currentParseResult.status = ParseResult::Status::UnknownError;
			return nullptr;
		}

		AstInterpreter interpreter(graph);
		interpreter.Visit(graph_def.get());

		/*
		std::string::const_iterator begin = std::begin(str);
		std::string::const_iterator end = std::end(str);

		GraphReaderGrammar<std::string::const_iterator> grammar;
		ast::graph_definition graph_def;

		bool result = qi::phrase_parse(begin, end, grammar, qi::space, graph_def);

		std::cout << "Result: " << result << std::endl;
		std::cout << "Processed all: " << (begin == end) << std::endl;
		std::cout << "Left to parse: " << std::endl;
		std::cout << std::string(begin, end) << std::endl;

		interpreter_visitor visitor{graph};
		visitor.visit(graph_def);

		*/
		return graph;
	}
	const ParseResult &GetParseResult() const { return m_currentParseResult; }

private:
	ParseResult m_currentParseResult;
};

GraphReader::GraphReader() : m_implementation(std::make_unique<GraphReader::Impl>()) {}
GraphReader::~GraphReader() {}
GraphPtr GraphReader::Read(const std::string &str) { return m_implementation->Read(str); }
const ParseResult &GraphReader::GetParseResult() const { return m_implementation->GetParseResult(); }

}  // namespace selector
