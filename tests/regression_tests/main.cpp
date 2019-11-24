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

REGRESSION_TEST(create_rect, "rect_x.obj", "rect_y.obj", "rect_z.obj")
REGRESSION_TEST(create_box, "geometry.obj")
REGRESSION_TEST(export_geometry, "geometry.obj")
REGRESSION_TEST(expression, "geometry.obj")
REGRESSION_TEST(extrusion, "geometry.obj")
REGRESSION_TEST(parameter_definition, "geometry.obj")
REGRESSION_TEST(parameter_overwrite, "geometry.obj")
REGRESSION_TEST(parameter_renaming, "geometry.obj")
REGRESSION_TEST(clip_geometry, "front.obj", "back.obj", "scope0.obj", "scope1.obj")
REGRESSION_TEST(triangulate_geometry, "geometry.obj")
REGRESSION_TEST(translate, "geometry.obj")
REGRESSION_TEST(repeat_split, "geometry_0.obj", "geometry_11.obj", "geometry_14.obj", "geometry_17.obj",
                "geometry_2.obj", "geometry_22.obj", "geometry_25.obj", "geometry_28.obj", "geometry_30.obj",
                "geometry_5.obj", "geometry_8.obj", "geometry_1.obj", "geometry_12.obj", "geometry_15.obj",
                "geometry_18.obj", "geometry_20.obj", "geometry_23.obj", "geometry_26.obj", "geometry_29.obj",
                "geometry_31.obj", "geometry_6.obj", "geometry_9.obj", "geometry_10.obj", "geometry_13.obj",
                "geometry_16.obj", "geometry_19.obj", "geometry_21.obj", "geometry_24.obj", "geometry_27.obj",
                "geometry_3.obj", "geometry_4.obj", "geometry_7.obj")
REGRESSION_TEST(extract_faces, "geometry0.obj", "geometry1.obj", "geometry2.obj", "geometry3.obj", "geometry4.obj",
                "geometry5.obj")
REGRESSION_TEST(split, "geometry0.obj", "geometry1.obj", "geometry2.obj")
REGRESSION_TEST(scale, "geometry_scope_center.obj", "geometry_scope_origin.obj", "geometry_world_origin.obj")
REGRESSION_TEST(rotate, "geometry.obj")
REGRESSION_TEST(parameters_in_procedural_objects, "geometry_0.obj", "geometry_1.obj", "geometry_2.obj",
                "geometry_3.obj", "geometry_4.obj", "geometry_5.obj", "geometry_6.obj", "geometry_7.obj",
                "geometry_8.obj", "geometry_9.obj");
/*
REGRESSION_TEST(banner, "geometry0.obj", "geometry107.obj", "geometry18.obj", "geometry27.obj", "geometry36.obj",
                "geometry45.obj", "geometry54.obj", "geometry63.obj", "geometry72.obj", "geometry81.obj",
                "geometry90.obj", "geometry1.obj", "geometry108.obj", "geometry19.obj", "geometry28.obj",
                "geometry37.obj", "geometry46.obj", "geometry55.obj", "geometry64.obj", "geometry73.obj",
                "geometry82.obj", "geometry91.obj", "geometry10.obj", "geometry109.obj", "geometry2.obj",
                "geometry29.obj", "geometry38.obj", "geometry47.obj", "geometry56.obj", "geometry65.obj",
                "geometry74.obj", "geometry83.obj", "geometry92.obj", "geometry100.obj", "geometry11.obj",
                "geometry20.obj", "geometry3.obj", "geometry39.obj", "geometry48.obj", "geometry57.obj",
                "geometry66.obj", "geometry75.obj", "geometry84.obj", "geometry93.obj", "geometry101.obj",
                "geometry12.obj", "geometry21.obj", "geometry30.obj", "geometry4.obj", "geometry49.obj",
                "geometry58.obj", "geometry67.obj", "geometry76.obj", "geometry85.obj", "geometry94.obj",
                "geometry102.obj", "geometry13.obj", "geometry22.obj", "geometry31.obj", "geometry40.obj",
                "geometry5.obj", "geometry59.obj", "geometry68.obj", "geometry77.obj", "geometry86.obj",
                "geometry95.obj", "geometry103.obj", "geometry14.obj", "geometry23.obj", "geometry32.obj",
                "geometry41.obj", "geometry50.obj", "geometry6.obj", "geometry69.obj", "geometry78.obj",
                "geometry87.obj", "geometry96.obj", "geometry104.obj", "geometry15.obj", "geometry24.obj",
                "geometry33.obj", "geometry42.obj", "geometry51.obj", "geometry60.obj", "geometry7.obj",
                "geometry79.obj", "geometry88.obj", "geometry97.obj", "geometry105.obj", "geometry16.obj",
                "geometry25.obj", "geometry34.obj", "geometry43.obj", "geometry52.obj", "geometry61.obj",
                "geometry70.obj", "geometry8.obj", "geometry89.obj", "geometry98.obj", "geometry106.obj",
                "geometry17.obj", "geometry26.obj", "geometry35.obj", "geometry44.obj", "geometry53.obj",
                "geometry62.obj", "geometry71.obj", "geometry80.obj", "geometry9.obj", "geometry99.obj")
                */

int main(int argc, char* argv[])
{
	RegressionTest::SetExecutablePath(argv[0]);
	::testing::InitGoogleTest(&argc, argv);

	auto returnVal = RUN_ALL_TESTS();

	ConsoleProfilerLogger consoleLogger(ProfilerManager::Instance());
	consoleLogger.Log(20);

	return returnVal;
}
