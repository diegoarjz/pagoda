#include <common/file_util.h>
#include <common/logger.h>
#include <common/profiler.h>
#include <geometry_core/geometry_exporter.h>
#include <parameter/parameter.h>
#include <procedural_graph/graph_dot_exporter.h>
#include <procedural_graph/input_interface_node.h>
#include <procedural_graph/node_set_visitor.h>
#include <procedural_graph/operation_node.h>
#include <procedural_graph/output_interface_node.h>
#include <procedural_graph/parameter_node.h>
#include <procedural_graph/parse_result.h>
#include <procedural_graph/reader.h>
#include <procedural_graph/default_scheduler.h>
#include <procedural_objects/export_geometry.h>
#include <procedural_objects/geometry_component.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/hierarchical_system.h>

#include <procedural_objects/create_rect.h>
#include <procedural_objects/extrude_geometry.h>
#include <procedural_objects/triangulate_geometry.h>

#include <selector.h>

#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>

namespace po = boost::program_options;
using namespace selector;

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm);
std::shared_ptr<Graph> ReadGraphFromFile(Selector &selector, const std::string& file_path);
void WriteDotFile(std::shared_ptr<Graph> graph, const std::string& file_path);
void ExecuteGraph(std::shared_ptr<Graph> graph);
void PrintProfile();

int main(int argc, char* argv[])
{
	po::variables_map vm;

    Selector selector;

	if (!ParseCommandLine(argc, argv, &vm))
	{
		return 0;
	}

	std::string file_path;
	std::string dot_file;
	try
	{
		if (vm.count("input-file"))
		{
			file_path = vm["input-file"].as<std::string>();
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
		std::shared_ptr<Graph> graph = ReadGraphFromFile(selector, file_path);
		if (graph == nullptr)
		{
			LOG_FATAL("Unable read a graph file (%s)", file_path.c_str());
			return 1;
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

	if (vm.count("show-profile"))
	{
		PrintProfile();
	}

	return 0;
}

void ExecuteGraph(std::shared_ptr<Graph> graph)
{
    graph->Execute();
}

std::shared_ptr<Graph> ReadGraphFromFile(Selector &selector, const std::string& file_path)
{
	return selector.CreateGraphFromFile(file_path);
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
            ("input-file", po::value<std::string>(), "Input Graph specification file.")
            ("dot", po::value<std::string>(), "Outputs the graph in dot format to the specified file.")
            ("execute", "Executes the graph")
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
