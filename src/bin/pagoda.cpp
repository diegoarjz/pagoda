#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/exception.h>
#include <pagoda/common/fs/file_util.h>
#include <pagoda/common/instrument/profiler.h>
#include <pagoda/common/version.h>

#include <pagoda/dynamic/set_value_from.h>
#include <pagoda/dynamic/value_visitor.h>

#include <pagoda/graph/default_scheduler.h>
#include <pagoda/graph/execution_queue.h>
#include <pagoda/graph/graph_dot_exporter.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/io/parse_result.h>
#include <pagoda/graph/io/reader.h>
#include <pagoda/graph/node_set_visitor.h>
#include <pagoda/graph/node_visitor.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/parameter_node.h>
#include <pagoda/graph/query/query.h>
#include <pagoda/graph/router_node.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/objects/hierarchical_system.h>
#include <pagoda/pagoda.h>

#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>
#include <regex>

namespace po = boost::program_options;
using namespace pagoda;
using namespace pagoda::objects;
using namespace pagoda::geometry;
using namespace pagoda::dynamic;
using namespace pagoda::graph;

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm);
std::shared_ptr<Graph> ReadGraphFromFile(Pagoda& pagoda, const std::string& file_path);
void SetParameter(const NodeSet& nodes, const std::string& param);
void WriteDotFile(std::shared_ptr<Graph> graph, const std::string& file_path);
void ListGraph(std::shared_ptr<Graph> graph);
void ExecuteGraph(std::shared_ptr<Graph> graph);
void PrintProfile();

int main(int argc, char* argv[])
{
	po::variables_map vm;

	Pagoda pagoda;

	if (!ParseCommandLine(argc, argv, &vm))
	{
		return 0;
	}

	std::string file_path;
	std::string dot_file;
	try
	{
		if (vm.count("file"))
		{
			file_path = vm["file"].as<std::string>();
		}
		if (vm.count("dot"))
		{
			dot_file = vm["dot"].as<std::string>();
		}
	}
	catch (po::error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	if (file_path.size() > 0)
	{
		try
		{
			std::shared_ptr<Graph> graph = ReadGraphFromFile(pagoda, file_path);
			if (graph == nullptr)
			{
				LOG_FATAL("Unable read a graph file (" << file_path << ")");
				return 1;
			}

			if (vm.count("param"))
			{
				std::vector<std::string> params = vm["param"].as<std::vector<std::string>>();
				NodeSet nodes;
				query::Query q(nodes);
				graph->ExecuteQuery(q);
				for (const auto& p : params)
				{
					SetParameter(nodes, p);
				}
			}

			if (vm.count("list"))
			{
				ListGraph(graph);
			}

			if (dot_file.size() > 0)
			{
				WriteDotFile(graph, dot_file);
			}

			if (vm.count("execute"))
			{
				ExecuteGraph(graph);
			}
		}
		catch (const pagoda::common::exception::Exception& e)
		{
			LOG_FATAL("Exception: " << e.What());
		}
	}

	if (vm.count("show-profile"))
	{
		PrintProfile();
	}

	return 0;
}

void ExecuteGraph(std::shared_ptr<Graph> graph) { graph->Execute(); }

std::shared_ptr<Graph> ReadGraphFromFile(Pagoda& pagoda, const std::string& file_path)
{
	return pagoda.CreateGraphFromFile(file_path);
}

struct PrintVisitor : NodeVisitor
{
	void Visit(std::shared_ptr<OperationNode> n) override { print(n, "OperationNode"); }
	void Visit(std::shared_ptr<InputInterfaceNode> n) override { print(n, "InputInterfaceNode"); }
	void Visit(std::shared_ptr<OutputInterfaceNode> n) override { print(n, "OutputInterfaceNode"); }
	void Visit(std::shared_ptr<ParameterNode> n) override { print(n, "ParameterNode"); }
	void Visit(std::shared_ptr<RouterNode> n) override { print(n, "RouterNode"); }

	void print(std::shared_ptr<Node> n, const std::string& nodeType)
	{
		std::cout << "  - name: " << n->GetName() << std::endl;
		std::cout << "    id: " << n->GetId() << std::endl;
		std::cout << "    type: " << nodeType << std::endl;
		auto parametersEnd = n->GetMembersEnd();
		std::cout << "    parameters:" << std::endl;
		for (auto iter = n->GetMembersBegin(); iter != parametersEnd; ++iter)
		{
			std::cout << "    - name: " << iter->first << std::endl;
			std::cout << "      type: " << iter->second.m_value->GetTypeInfo()->GetTypeName() << std::endl;
			std::cout << "      value: " << iter->second.m_value->ToString() << std::endl;
		}
	}
};

void ListGraph(std::shared_ptr<Graph> graph)
{
	ExecutionQueue q(*graph);
	auto n = q.GetNextNode();
	std::cout << "nodes:" << std::endl;

	PrintVisitor v;
	do
	{
		n->AcceptNodeVisitor(&v);
		n = q.GetNextNode();
	} while (n != nullptr);
}

struct ParamSetter : ValueVisitorBase
{
	ParamSetter(const std::string& v) : m_value(v) {}

	void Visit(Boolean& v) override
	{
		if (m_value != "true" || m_value != "false")
		{
			throw pagoda::common::exception::Exception("Unable to set Boolean parameter from " + m_value + " value.");
		}
		set_value_from<bool>(v, m_value == "true");
	}
	void Visit(FloatValue& v) override { set_value_from<float>(v, std::atof(m_value.c_str())); }
	void Visit(Integer& v) override { set_value_from<float>(v, std::atof(m_value.c_str())); }
	void Visit(String& v) override { set_value_from<std::string>(v, m_value); }
	void Visit(NullObject& v) override { throw pagoda::common::exception::Exception("Cannot set a NullObject."); }
	void Visit(TypeInfo& v) override { throw pagoda::common::exception::Exception("Cannot set a TypeInfo."); }
	void Visit(Vector3& v) override { throw pagoda::common::exception::Exception("Cannot set a Vector3."); }
	void Visit(DynamicPlane& v) override { throw pagoda::common::exception::Exception("Cannot set a DynamicPlane."); }
	void Visit(Function& v) override { throw pagoda::common::exception::Exception("Cannot set a Function."); }
	void Visit(DynamicClass& v) override { throw pagoda::common::exception::Exception("Cannot set a DynamicClass."); }
	void Visit(DynamicInstance& v) override
	{
		throw pagoda::common::exception::Exception("Cannot set a DynamicInstance.");
	}
	void Visit(Expression& v) override { throw pagoda::common::exception::Exception("Cannot set an Expression."); }
	void Visit(ProceduralOperation& v) override
	{
		throw pagoda::common::exception::Exception("Cannot set a ProceduralOperation.");
	}

	std::string m_value;
};

void SetParameter(const NodeSet& nodes, const std::string& param)
{
	static const std::regex paramRegex("^(.+)\\.(.+)=(.+)$");
	std::smatch matches;
	if (std::regex_search(param, matches, paramRegex) && matches.size() > 3)
	{
		std::string nodeName = matches.str(1);
		std::string paramName = matches.str(2);
		std::string value = matches.str(3);
		ParamSetter setter(value);
		for (auto& n : nodes)
		{
			if (n->GetName() == nodeName)
			{
				LOG_INFO("Overriding parameter '" << paramName << "' in node '" << n->GetName() << "' with value '"
				                                  << value << "'");
				n->GetMember(paramName)->AcceptVisitor(setter);
			}
		}
	}
	else
	{
		throw pagoda::common::exception::Exception("Invalid parameter definition: '" + param + "'");
	}
}

void WriteDotFile(std::shared_ptr<Graph> graph, const std::string& file_path)
{
	std::ofstream outFile(file_path);

	GraphDotExporter exporter(graph);
	exporter.SetRankBy(GraphDotExporter::RankBy::Depth);
	exporter.SetShowParameters(true);
	exporter.Export(outFile);
}

void PrintProfile()
{
	if (!pagoda::common::has_feature("Profiler"))
	{
		LOG_WARNING("Pagoda was compiled without the Profiler feature.");
		LOG_WARNING(" Please build Pagoda with '-DPAGODA_PROFILER_ACTIVE=ON'.");
		return;
	}

	std::cout << "Showing profiling data:" << std::endl;
	pagoda::common::instrument::ConsoleProfilerLogger consoleLogger(
	    pagoda::common::instrument::ProfilerManager::Instance());
	consoleLogger.Log(20);
}

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm)
{
	try
	{
		po::positional_options_description positionalOptions;
		positionalOptions.add("file", 1);

		po::options_description desc("Options");
		// clang-format off
        desc.add_options()
            ("help", "Print help message.")
            ("version", "Print version information and exit.")
            ("file", po::value<std::string>(), "Input Graph specification file.")
            ("dot", po::value<std::string>(), "Outputs the graph in dot format to the specified file.")
            ("execute", "Executes the graph")
            ("list", "Lists all nodes and parameters in a graph")
            ("param", po::value<std::vector<std::string>>(), "Override a parameter in a node.\nFormat: '<node name>.<param name>=<value>'")
            ("show-profile", "Prints profiling information");
		// clang-format on

		po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(), *out_vm);
		po::notify(*out_vm);

		if (out_vm->count("help"))
		{
			std::cout << "Pagoda command line tool" << std::endl << desc << std::endl;
			return false;
		}

		if (out_vm->count("version"))
		{
			std::cout << pagoda::common::get_version_information() << std::endl;
			return false;
		}
	}
	catch (po::error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
	catch (std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}

	return true;
}
