#include "reader.h"

#include "common/assertions.h"
#include "common/logger.h"
#include "graph_reader_grammar.h"
#include "input_interface_node.h"
#include "node.h"
#include "operation_node.h"
#include "output_interface_node.h"
#include "parse_result.h"

#include "parameter/context.h"

#include "procedural_objects/procedural_operation.h"

#include <cstring>
#include <exception>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace selector
{
struct Callable;
using CallablePtr = std::shared_ptr<Callable>;

using builtin_value = boost::variant<float, std::string, NodePtr, CallablePtr>;

struct Callable
{
	virtual ~Callable() {}
	virtual builtin_value Call(std::unordered_map<std::string, builtin_value> &) = 0;
};

struct OperationNodeCreator : public Callable
{
	virtual ~OperationNodeCreator() {}
	builtin_value Call(std::unordered_map<std::string, builtin_value> &constructionArgs)
	{
		LOG_TRACE(ProceduralGraphReader, "Creating Operation Node");
		auto node = Node::Create("Operation");

		if (node == nullptr)
		{
			return node;
		}

		auto operationNode = std::dynamic_pointer_cast<OperationNode>(node);
		auto operationType = constructionArgs.find("operation");
		if (operationType == std::end(constructionArgs))
		{
			return node;
		}

		auto type = boost::get<std::string>(operationType->second);
		LOG_TRACE(ProceduralGraphReader, "\tType: %s", type.c_str());
		operationNode->SetName(type);
		operationNode->SetOperation(ProceduralOperation::Create(type));

		return node;
	}
};

struct InputInterfaceNodeCreator : public Callable
{
	virtual ~InputInterfaceNodeCreator() {}
	builtin_value Call(std::unordered_map<std::string, builtin_value> &constructionArgs)
	{
		LOG_TRACE(ProceduralGraphReader, "Creating Input Interface Node");
		auto node = Node::Create("InputInterface");

		if (node == nullptr)
		{
			return node;
		}

		auto interfaceNode = std::dynamic_pointer_cast<InputInterfaceNode>(node);
		auto interfaceName = boost::get<std::string>(constructionArgs["interface"]);
		auto interfaceOffset = boost::get<float>(constructionArgs["offset"]);
		interfaceNode->SetName(interfaceName);
		LOG_TRACE(ProceduralGraphReader, "\tname %s offset: %f", interfaceName.c_str(), interfaceOffset);

		// TODO: interface name and offset
		interfaceNode->SetInterfaceName(InterfaceName(interfaceName.c_str(), interfaceOffset));

		return node;
	}
};

struct OutputInterfaceNodeCreator : public Callable
{
	virtual ~OutputInterfaceNodeCreator() {}
	builtin_value Call(std::unordered_map<std::string, builtin_value> &constructionArgs)
	{
		LOG_TRACE(ProceduralGraphReader, "Creating Output Interface Node");
		auto node = Node::Create("OutputInterface");

		if (node == nullptr)
		{
			return node;
		}

		auto interfaceNode = std::dynamic_pointer_cast<OutputInterfaceNode>(node);
		auto interfaceName = boost::get<std::string>(constructionArgs["interface"]);
		auto interfaceOffset = boost::get<float>(constructionArgs["offset"]);
		interfaceNode->SetName(interfaceName);
		LOG_TRACE(ProceduralGraphReader, "\tname %s offset: %f", interfaceName.c_str(), interfaceOffset);

		// TODO: interface name and offset
		interfaceNode->SetInterfaceName(InterfaceName(interfaceName.c_str(), interfaceOffset));

		return node;
	}
};

struct Symbol
{
	std::string m_name;
	builtin_value m_value;
};

class SymbolTable
{
public:
	struct SymbolNotFound : public std::exception
	{
		SymbolNotFound(const std::string &symbolName) : m_symbolName(symbolName) {}
		SymbolNotFound() = delete;

		std::string m_symbolName;
	};

	SymbolTable() = default;
	SymbolTable(std::shared_ptr<SymbolTable> parentScope) : m_parentScope(parentScope) {}
	SymbolTable(const SymbolTable &) = delete;
	SymbolTable &operator=(const SymbolTable &) = delete;

	const Symbol &GetSymbol(const std::string &name)
	{
		auto iter = m_symbols.find(name);
		if (iter != std::end(m_symbols))
		{
			return iter->second;
		}
		if (!m_parentScope.expired())
		{
			return m_parentScope.lock()->GetSymbol(name);
		}

		throw SymbolNotFound(name);
	}

	void AddSymbol(const Symbol &symbol) { m_symbols[symbol.m_name] = symbol; }

private:
	std::weak_ptr<SymbolTable> m_parentScope;
	std::unordered_map<std::string, Symbol> m_symbols;
};

namespace operations
{
struct base_operation
{
};

struct add : public base_operation
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs + rhs; }
	static builtin_value apply(const std::string &lhs, const std::string &rhs) { return lhs + rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to add operands");
	}
};

struct sub : public base_operation
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs - rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to subtract operands");
	}
};

struct mul : public base_operation
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs * rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to multiply operands");
	}
};

struct div : public base_operation
{
	static builtin_value apply(const float &lhs, const float &rhs) { return lhs / rhs; }

	template<typename LHS, typename RHS>
	static builtin_value apply(const LHS &lhs, const RHS &rhs)
	{
		throw std::runtime_error("Unable to divide operands");
	}
};
}  // namespace operations

template<class OP>
struct binary_op_dispatcher : public boost::static_visitor<builtin_value>
{
	binary_op_dispatcher(const builtin_value &lhs, const builtin_value &rhs) : m_lhs(lhs), m_rhs(rhs) {}
	binary_op_dispatcher() = delete;

	template<typename LHS>
	struct rhs_dispatcher : public boost::static_visitor<builtin_value>
	{
		rhs_dispatcher(const LHS &lhs) : m_lhs(lhs) {}
		rhs_dispatcher() = delete;

		template<typename RHS>
		builtin_value operator()(const RHS &rhs)
		{
			return OP::apply(m_lhs, rhs);
		}

	private:
		const LHS &m_lhs;
	};

	template<typename LHS>
	builtin_value operator()(const LHS &lhs)
	{
		rhs_dispatcher<LHS> dispatcher{lhs};
		return boost::apply_visitor(dispatcher, m_rhs);
	}

private:
	const builtin_value &m_lhs;
	const builtin_value &m_rhs;
};

struct interpreter_visitor
{
	interpreter_visitor(GraphPtr graph) : m_symbolTable(std::make_shared<SymbolTable>()), m_graph(graph)
	{
		m_symbolTable->AddSymbol({"Operation", std::make_shared<OperationNodeCreator>()});
		m_symbolTable->AddSymbol({"InputInterface", std::make_shared<InputInterfaceNodeCreator>()});
		m_symbolTable->AddSymbol({"OutputInterface", std::make_shared<OutputInterfaceNodeCreator>()});
	}

	void visit(const ast::graph_definition &graphDef)
	{
		LOG_TRACE(ProceduralGraphReader, "Processing Graph Definition");
		struct statement_visitor : boost::static_visitor<void>
		{
			statement_visitor(interpreter_visitor *v) : m_visitor(v) {}
			void operator()(const ast::assignment &assignment) { m_visitor->visit(assignment); }
			void operator()(const ast::node_links &links) { m_visitor->visit(links); }

			interpreter_visitor *m_visitor;
		};
		statement_visitor v{this};

		for (const auto &statement : graphDef.m_statements)
		{
			boost::apply_visitor(v, statement);
		}
	}

	void visit(const ast::assignment &assignment)
	{
		LOG_TRACE(ProceduralGraphReader, "Processing Assignment");
		m_symbolTable->AddSymbol({assignment.m_lhs.m_name, visit(assignment.m_rhs)});
	}

	void visit(const ast::node_links &node_links)
	{
		LOG_TRACE(ProceduralGraphReader, "Processing Node Links");
		auto prevNode = boost::get<NodePtr>(visit(node_links[0]));
		for (auto i = 1u; i < node_links.size(); ++i)
		{
			auto nextNode = boost::get<NodePtr>(visit(node_links[i]));
			LOG_TRACE(ProceduralGraphReader, "\t%d -> %d", prevNode->GetId(), nextNode->GetId());
			m_graph->CreateEdge(prevNode, nextNode);
			prevNode = nextNode;
		}
	}

	builtin_value visit(const ast::expression &expression)
	{
		LOG_TRACE(ProceduralGraphReader, "Processing Expression");
		struct expression_visitor : boost::static_visitor<builtin_value>
		{
			expression_visitor(interpreter_visitor *v) : m_visitor(v) {}

			builtin_value operator()(const ast::literal &literal) { return m_visitor->visit(literal); }
			builtin_value operator()(const ast::variable &variable) { return m_visitor->visit(variable); }
			builtin_value operator()(const ast::binary_op &binary_op) { return m_visitor->visit(binary_op); }
			builtin_value operator()(const ast::node_definition &node_definition)
			{
				return m_visitor->visit(node_definition);
			}

			interpreter_visitor *m_visitor;
		};

		expression_visitor v{this};
		return boost::apply_visitor(v, expression);
	}

	builtin_value visit(const ast::literal &literal)
	{
		LOG_TRACE(ProceduralGraphReader, "Processing Literal");
		return literal;
	}

	builtin_value visit(const ast::variable &variable)
	{
		LOG_TRACE(ProceduralGraphReader, "Processing Variable");
		try
		{
			return m_symbolTable->GetSymbol(variable.m_name).m_value;
		}
		catch (SymbolTable::SymbolNotFound s)
		{
			throw std::move(s);  // TODO:
		}
	}

	builtin_value visit(const ast::binary_op &op)
	{
		auto lhs = visit(op.m_lhs);
		auto rhs = visit(op.m_rhs);
		switch (op.m_operation)
		{
			case '+':
			{
				LOG_TRACE(ProceduralGraphReader, "Processing Binary Operation +");
				binary_op_dispatcher<operations::add> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case '-':
			{
				LOG_TRACE(ProceduralGraphReader, "Processing Binary Operation -");
				binary_op_dispatcher<operations::sub> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case '*':
			{
				LOG_TRACE(ProceduralGraphReader, "Processing Binary Operation *");
				binary_op_dispatcher<operations::mul> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			}
			case '/':
			{
				LOG_TRACE(ProceduralGraphReader, "Processing Binary Operation /");
				binary_op_dispatcher<operations::div> v(lhs, rhs);
				return boost::apply_visitor(v, lhs);
			};
		}
		throw std::runtime_error("Unknown operation");
	}

	builtin_value visit(const ast::node_definition &node_definition)
	{
		LOG_TRACE(ProceduralGraphReader, "Processing Node Definition");
		auto callable = boost::get<CallablePtr>(m_symbolTable->GetSymbol(node_definition.m_nodeType).m_value);
		auto constructionArgs = visit(node_definition.m_constructionArguments);
		auto nodeParameters = visit(node_definition.m_nodeParameters);
		auto node = boost::get<NodePtr>(callable->Call(constructionArgs));

		auto context = std::make_shared<Context>(node_definition.m_nodeType);
		node->SetParameterContext(context);

		struct parameter_setter : public boost::static_visitor<void>
		{
			parameter_setter(std::shared_ptr<Context> c) : m_context(c) {}

			void operator()(const float &v) { m_context->CreateParameter<float>(m_parameterName, v); }
			void operator()(const std::string &) {}
			void operator()(const NodePtr &) {}
			void operator()(const CallablePtr &v) {}

			std::shared_ptr<Context> m_context;
			std::string m_parameterName;
		};

		parameter_setter v{context};
		for (auto n : nodeParameters)
		{
			v.m_parameterName = n.first;
			boost::apply_visitor(v, n.second);
		}

		m_graph->AddNode(node);

		return node;
	}

	std::unordered_map<std::string, builtin_value> visit(const ast::named_arg_list &arg_list)
	{
		LOG_TRACE(ProceduralGraphReader, "Processing Named Arg List");
		std::unordered_map<std::string, builtin_value> values;

		for (const auto &arg : arg_list)
		{
			auto value = visit(arg.m_value);
			auto inserted = values.insert(std::make_pair(arg.m_name, value));
			if (!inserted.second)
			{
				throw std::runtime_error("Redefinition of named parameter");
			}
		}

		return values;
	}

private:
	std::shared_ptr<SymbolTable> m_symbolTable;
	GraphPtr m_graph;
};

struct GraphReader::Impl
{
	Impl() : m_currentParseResult({ParseResult::Status::Ok, 0}) {}
	GraphPtr Read(const std::string &str)
	{
		GraphPtr graph = std::make_shared<Graph>();
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
