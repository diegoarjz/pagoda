#include <gtest/gtest.h>

#include <common/file_util.h>
#include <common/logger.h>
#include <common/profiler.h>
#include <procedural_graph/default_scheduler.h>
#include <procedural_graph/graph.h>
#include <procedural_graph/reader.h>
#include <procedural_objects/geometry_system.h>
#include <procedural_objects/hierarchical_system.h>
#include <procedural_objects/procedural_object.h>

#include <selector.h>

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
		m_graph = m_selector.CreateGraphFromFile(filePath);
		return m_graph;
	}

	void ExecuteGraph() { m_graph->Execute(); }

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
	Selector m_selector;

	static boost::filesystem::path s_testFilesDirectory;
};
boost::filesystem::path RegressionTest::s_testFilesDirectory = "";

#define REGRESSION_TEST(NAME, ...) \
	TEST(RegressionTestCase, NAME) { RegressionTest(#NAME, {__VA_ARGS__}); }

REGRESSION_TEST(create_rect, "geometry.obj")
REGRESSION_TEST(create_box, "geometry.obj")
REGRESSION_TEST(export_geometry, "geometry.obj")
REGRESSION_TEST(expression, "geometry.obj")
REGRESSION_TEST(extrusion, "geometry.obj")
REGRESSION_TEST(parameter_definition, "geometry.obj")
REGRESSION_TEST(parameter_overwrite, "geometry.obj")
REGRESSION_TEST(parameter_renaming, "geometry.obj")
REGRESSION_TEST(clip_geometry, "front.obj", "back.obj", "scope0.obj", "scope1.obj")
REGRESSION_TEST(triangulate_geometry, "geometry.obj")
REGRESSION_TEST(repeat_split, "geometry_0.obj", "geometry_11.obj", "geometry_14.obj", "geometry_17.obj",
                "geometry_2.obj", "geometry_22.obj", "geometry_25.obj", "geometry_28.obj", "geometry_30.obj",
                "geometry_5.obj", "geometry_8.obj", "geometry_1.obj", "geometry_12.obj", "geometry_15.obj",
                "geometry_18.obj", "geometry_20.obj", "geometry_23.obj", "geometry_26.obj", "geometry_29.obj",
                "geometry_31.obj", "geometry_6.obj", "geometry_9.obj", "geometry_10.obj", "geometry_13.obj",
                "geometry_16.obj", "geometry_19.obj", "geometry_21.obj", "geometry_24.obj", "geometry_27.obj",
                "geometry_3.obj", "geometry_4.obj", "geometry_7.obj")
REGRESSION_TEST(banner, "geometry0.obj", "geometry1.obj", "geometry2.obj")

int main(int argc, char* argv[])
{
	RegressionTest::SetExecutablePath(argv[0]);
	::testing::InitGoogleTest(&argc, argv);

	auto returnVal = RUN_ALL_TESTS();

	ConsoleProfilerLogger consoleLogger(ProfilerManager::Instance());
	consoleLogger.Log(20);

	return returnVal;
}
