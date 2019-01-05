#include <common/logger.h>
#include <common/profiler.h>
#include <geometry_core/geometry_exporter.h>
#include <parameter/parameter.h>
#include <procedural_graph/graph_execution_context.h>
#include <procedural_graph/node_execution_result.h>
#include <procedural_graph/node_type.h>
#include <procedural_graph/output_interface_execution.h>
#include <procedural_graph/reader.h>
#include <procedural_graph/scheduler.h>
#include <procedural_graph/xml_parse_result.h>
#include <procedural_objects/geometry_component.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/hierarchical_system.h>

#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>

namespace po = boost::program_options;
using namespace selector;

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm);
std::shared_ptr<Graph> ReadGraphFromFile(const std::string& file_path);
void WriteDotFile(std::shared_ptr<Graph> graph, const std::string& file_path);
std::list<ProceduralObjectPtr> ExecuteGraph(std::shared_ptr<Graph> graph,
                                            std::shared_ptr<GraphExecutionContext> execution_context);
void PrintProfile();
const char* XMLReaderParseMessage(selector::ParseResult::Status status);

int main(int argc, char* argv[])
{
	po::variables_map vm;

	if (!ParseCommandLine(argc, argv, &vm))
	{
		Logger::Shutdown();
		return 0;
	}

	std::string file_path;
	std::string dot_file;
	try
	{
		file_path = vm["input-file"].as<std::string>();
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

	std::shared_ptr<Graph> graph = ReadGraphFromFile(file_path);
	if (graph == nullptr)
	{
		LOG_FATAL("Unable read a graph file (%s)", file_path.c_str());
		Logger::Shutdown();
		return 1;
	}

	if (dot_file.size() > 0)
	{
		WriteDotFile(graph, dot_file);
	}

	if (vm.count("execute"))
	{
		auto geom_system = std::make_shared<GeometrySystem>();
		auto hierarchical_system = std::make_shared<HierarchicalSystem>();
		auto object_system = std::make_shared<ProceduralObjectSystem>();
		object_system->RegisterProceduralComponentSystem(geom_system);
		object_system->RegisterProceduralComponentSystem(hierarchical_system);
		auto execution_context = std::make_shared<GraphExecutionContext>(graph, object_system, geom_system);

		std::list<ProceduralObjectPtr> procedural_objects = ExecuteGraph(graph, execution_context);

		if (vm.count("export-geometry"))
		{
			std::string export_path = ".";
			if (vm.count("export-path"))
			{
				export_path = vm["export-path"].as<std::string>();
			}

			uint32_t geometry_index = 0;
			for (auto o : procedural_objects)
			{
				auto geometry_component = o->GetComponent<GeometryComponent>();
				auto geometry = geometry_component->GetGeometry();
				selector::ObjExporter<Geometry> exporter(geometry);

				std::stringstream geometry_path;
				geometry_path << export_path << "/geom" << geometry_index << ".obj";

				std::ofstream out_file(geometry_path.str());
				exporter.Export(out_file);
				out_file.close();
				geometry_index++;
			}
		}

		// Clean up
		std::unordered_set<ProceduralObjectPtr> objects = object_system->GetProceduralObjects();
		for (ProceduralObjectPtr o : objects)
		{
			object_system->KillProceduralObject(o);
		}
	}

	if (vm.count("show-profile"))
	{
		PrintProfile();
	}

	Logger::Shutdown();
	return 0;
}

std::list<ProceduralObjectPtr> ExecuteGraph(std::shared_ptr<Graph> graph,
                                            std::shared_ptr<GraphExecutionContext> executionContext)
{
	auto graph_parameter_context = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("graph").second);

	Scheduler scheduler(executionContext, graph_parameter_context);
	scheduler.Initialize();

	while (scheduler.Step())
	{
	}

	std::list<ProceduralObjectPtr> procedural_objects;
	std::list<NodePtr> output_nodes = graph->GetGraphOutputNodes();
	for (auto n : output_nodes)
	{
		if (n->GetNodeType() == NodeType::OutputInterface)
		{
			auto result = n->GetResult()->GetResultAs<OutputInterfaceExecution::Result>();
			for (const auto proceduralObject : *result->m_proceduralObjects)
			{
				procedural_objects.push_back(proceduralObject);
			}
		}
	}

	return procedural_objects;
}

std::shared_ptr<Graph> ReadGraphFromFile(const std::string& file_path)
{
	// TODO: this really is not good code...
	std::ifstream in_file(file_path);
	if (!in_file)
	{
		LOG_ERROR("Unable to open graph file %s", file_path.c_str());
		return nullptr;
	}

	std::string str;

	in_file.seekg(0, std::ios::end);
	str.reserve(in_file.tellg());
	in_file.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());
	XMLReader reader;
	auto read_result = reader.Read(str);

	if (read_result.status != ParseResult::Status::Ok)
	{
		LOG_ERROR("Unable to parse graph file: %s", file_path.c_str());
		LOG_ERROR("  Status Message: %s", XMLReaderParseMessage(read_result.status));
		LOG_ERROR("  Offset: %d", read_result.offset);
		return nullptr;
	}

	return reader.GetGraph();
}

void WriteDotFile(std::shared_ptr<Graph> graph, const std::string& file_path)
{
	std::ofstream out_file(file_path);

	out_file << "digraph {" << std::endl;
	out_file << "\trankdir=LR;" << std::endl;
	out_file << "\tnode [shape=plaintext fontname=\"Sans serif\" fontsize=\"8\"];" << std::endl;

	auto operation_nodes = graph->GetGraphNodesByType(NodeType::Operation);
	for (auto n : operation_nodes)
	{
		auto parameters = n->GetParameterContext()->GetParameters();

		out_file << "\t" << n->GetName() << " [ label=<\n";
		out_file << "<table border=\"1\" cellborder=\"0\" cellspacing=\"1\">\n";
		out_file << "<tr><td align=\"left\"><b>" << n->GetName() << "</b></td></tr>\n";

		for (auto par : parameters)
		{
			if (par.second->GetExpression() == nullptr)
			{
				out_file << "<tr><td>" << par.second->Name() << "</td></tr>\n";
			}
			else
			{
				out_file << "<tr><td>" << par.second->Name() << "(e)</td></tr>\n";
			}
		}

		out_file << "</table>>]\n\n";
	}

	for (uint32_t type = 0; type < Index(NodeType::MAX); ++type)
	{
		auto nodes = graph->GetGraphNodesByType(static_cast<NodeType>(type));
		for (auto n : nodes)
		{
			auto out_nodes = graph->GetNodeOutputNodes(n);
			std::string source_node_name = n->GetName();

			for (auto out_node : out_nodes)
			{
				std::string target_node_name = out_node->GetName();
				out_file << "\t" << source_node_name << " -> " << target_node_name << ";" << std::endl;
			}
		}
	}

	out_file << "}";
}

const char* XMLReaderParseMessage(selector::ParseResult::Status status)
{
	switch (status)
	{
		case selector::ParseResult::Status::Ok:
			return "Ok";
		case selector::ParseResult::Status::FileNotFound:
			return "File Not Found";
		case selector::ParseResult::Status::IOError:
			return "I/O Error";
		case selector::ParseResult::Status::OutOfMemory:
			return "Out of Memory";
		case selector::ParseResult::Status::InternalError:
			return "Internal Error";
		case selector::ParseResult::Status::UnrecognizedTag:
			return "Unrecognized Tag";
		case selector::ParseResult::Status::BadElement:
			return "Bad Element";
		case selector::ParseResult::Status::EndElementMismatch:
			return "End Element Mismatch";
		case selector::ParseResult::Status::NoDocumentElement:
			return "No Document Element";
		case selector::ParseResult::Status::VersionMismatch:
			return "Version Mismatch";
		case selector::ParseResult::Status::GraphElementNotFound:
			return "Graph Element Not Found";
		case selector::ParseResult::Status::VersionElementNotFound:
			return "Version Element Not Found";
		case selector::ParseResult::Status::DuplicatedNodeId:
			return "Duplicated Node ID";
		case selector::ParseResult::Status::UnknownNodeType:
			return "Unknown Node Type";
		case selector::ParseResult::Status::InvalidParameter:
			return "Invalid Parameter";
		case selector::ParseResult::Status::UnknownError:
			return "Unknown Error";
		default:
			return "??";
	};
}

void PrintProfile()
{
	ConsoleProfilerLogger consoleLogger(ProfilerManager::Instance());
	consoleLogger.Log();
}

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm)
{
	try
	{
		po::options_description desc("Options");
		// clang-format off
        desc.add_options()
            ("help", "Print help message.")
            ("input-file", po::value<std::string>()->required(), "Input Graph specification file.")
            ("dot", po::value<std::string>(), "Outputs the graph in dot format to the specified file.")
            ("execute", "Executes the graph")
            ("export-geometry", "Exports the geometry generated by executing the graph")
            ("export-path", po::value<std::string>(), "Path to which geometries are exported")
            ("show-profile", "Prints profiling information");
		// clang-format on

		po::store(po::parse_command_line(argc, argv, desc), *out_vm);
		po::notify(*out_vm);

		if (out_vm->count("help"))
		{
			std::cout << "Procedural Graph Reader" << std::endl << desc << std::endl;
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
