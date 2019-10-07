#include <gtest/gtest.h>

#include <common/file_util.h>
#include <common/logger.h>
#include <procedural_graph/graph.h>
#include <procedural_graph/graph_execution_context.h>
#include <procedural_graph/reader.h>
#include <procedural_graph/scheduler.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/hierarchical_system.h>
#include <procedural_objects/procedural_object.h>

#include <procedural_graph/input_interface_node.h>
#include <procedural_graph/operation_node.h>
#include <procedural_graph/output_interface_node.h>
#include <procedural_graph/parameter_node.h>

#include <procedural_objects/create_rect.h>
#include <procedural_objects/export_geometry.h>
#include <procedural_objects/extrude_geometry.h>
#include <procedural_objects/triangulate_geometry.h>

#include <boost/filesystem/path.hpp>

using namespace selector;

class RegressionTest
{
public:
	static void SetExecutablePath(const std::string& path)
	{
		s_testFilesDirectory = boost::filesystem::path(path).remove_filename();
	}

	static boost::filesystem::path GetTestFilesDirectory() { return s_testFilesDirectory; }

	RegressionTest(const std::string& name, const std::vector<std::string>& filesToMatch)
	    : m_regressionTestName(name), m_filesToMatch(filesToMatch)
	{
		ReadGraphFromFile(GetInputGraph().string());
		ExecuteGraph();
		MatchFiles();
	}

	boost::filesystem::path GetInputGraph()
	{
		auto pathToGraphFile = GetTestFilesDirectory();
		pathToGraphFile /= "test_files";
		pathToGraphFile /= m_regressionTestName;
		pathToGraphFile /= m_regressionTestName + ".sel";
		return pathToGraphFile;
	}

	boost::filesystem::path GetExpectedResultFile(const std::string& expectedFile)
	{
		auto pathToExpectedFile = GetTestFilesDirectory();
		pathToExpectedFile /= "test_files";
		pathToExpectedFile /= m_regressionTestName;
		pathToExpectedFile /= expectedFile;
		return pathToExpectedFile;
	}

	std::shared_ptr<Graph> ReadGraphFromFile(const std::string& filePath)
	{
		std::string str = FileUtil::LoadFileToString(filePath);
		GraphReader reader;
		m_graph = reader.Read(str);
		return m_graph;
	}

	void ExecuteGraph()
	{
		auto geomSystem = std::make_shared<GeometrySystem>();
		auto hierarchicalSystem = std::make_shared<HierarchicalSystem>();
		auto objectSystem = std::make_shared<ProceduralObjectSystem>();
		objectSystem->RegisterProceduralComponentSystem(geomSystem);
		objectSystem->RegisterProceduralComponentSystem(hierarchicalSystem);

		auto executionContext = std::make_shared<GraphExecutionContext>(m_graph, objectSystem, geomSystem);
		auto graphParameterContext = std::make_shared<Context>("graph");

		Scheduler scheduler(executionContext, graphParameterContext);
		scheduler.Initialize();

		while (true)
		{
			if (!scheduler.Step())
			{
				break;
			}
		}
	}

	void MatchFiles()
	{
		for (const auto& f : m_filesToMatch)
		{
			std::string expectedFile = FileUtil::LoadFileToString(GetExpectedResultFile(f).string());
			std::string resultFile = FileUtil::LoadFileToString(f);
			EXPECT_EQ(expectedFile, resultFile);
		}
	}

private:
	std::string m_regressionTestName;
	std::vector<std::string> m_filesToMatch;

	GraphPtr m_graph;

	static boost::filesystem::path s_testFilesDirectory;
};
boost::filesystem::path RegressionTest::s_testFilesDirectory = "";

#define REGRESSION_TEST(NAME, FILES_TO_MATCH) \
	TEST(RegressionTestCase, NAME) { RegressionTest(#NAME, FILES_TO_MATCH); }

REGRESSION_TEST(create_rect, {"geometry.obj"});
REGRESSION_TEST(export_geometry, {"geometry.obj"});
REGRESSION_TEST(expression, {"geometry.obj"});
REGRESSION_TEST(extrusion, {"geometry.obj"});
REGRESSION_TEST(parameter_definition, {"geometry.obj"});
REGRESSION_TEST(parameter_overwrite, {"geometry.obj"});
REGRESSION_TEST(parameter_renaming, {"geometry.obj"});

int main(int argc, char* argv[])
{
	IsRegistered<OperationNode>();
	IsRegistered<InputInterfaceNode>();
	IsRegistered<OutputInterfaceNode>();
	IsRegistered<ParameterNode>();

	IsRegistered<ExtrudeGeometry>();
	IsRegistered<CreateRectGeometry>();
	IsRegistered<TriangulateGeometry>();
	IsRegistered<ExportGeometry>();

	RegressionTest::SetExecutablePath(argv[0]);
	::testing::InitGoogleTest(&argc, argv);

	auto returnVal = RUN_ALL_TESTS();

	selector::Logger::Shutdown();

	return returnVal;
}
