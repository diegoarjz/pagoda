#include <gtest/gtest.h>

#include <pagoda/pagoda.h>
#include <pagoda/pgscript/ir/ast.h>
#include <pagoda/pgscript/ir/ast_printer.h>
#include <pagoda/pgscript/interpreter/interpreter.h>
#include <pagoda/pgscript/parser/parser.h>

#include <pagoda/dynamic/dynamic_plane.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/integer_value.h>
#include <pagoda/dynamic/string_value.h>
#include <pagoda/dynamic/vector3.h>

#include <pagoda/math/vec_base.h>

#include <pagoda/common/exception/exception.h>
#include <pagoda/common/fs/file_util.h>

#include <boost/filesystem/path.hpp>

using namespace pagoda;
using namespace pagoda::common;
using namespace pagoda::math;
using namespace pagoda::dynamic;

class RegressionTest
{
public:
	static void SetExecutablePath(const std::string& path)
	{
		s_testFilesDirectory = boost::filesystem::path(path).remove_filename();
	}

	static boost::filesystem::path GetTestFilesDirectory() { return s_testFilesDirectory; }

	static void SetWriteOutput(bool write) { s_write = write; }

	RegressionTest(const std::string& name) : m_regressionTestName(name)
	{
		m_testDir = GetTestFilesDirectory() / "test_files" / name;
		std::string code = fs::LoadFileToString(GetScriptFile().string());
		m_program = Parser().Parse(code);
	}

	template<class T>
	void Match(std::shared_ptr<T> expectedValue)
	{
		std::stringstream myStdout;
		std::stringstream myStderr;
		Interpreter::SetStdOutStream(&myStdout);
		Interpreter::SetStdErrStream(&myStderr);

		m_interpreter.Interpret(m_program);
		auto last = std::dynamic_pointer_cast<T>(m_interpreter.GetLastEvaluatedExpression());
		ASSERT_NE(last, nullptr);
		ASSERT_EQ(*last, *expectedValue);

		std::stringstream ss;
		AstPrinter printer(ss);
		m_program->AcceptVisitor(&printer);
		if (s_write)
		{
			fs::WriteStringToFile(GetASTFile().string(), ss.str());
			fs::WriteStringToFile(GetStdOutFile().string(), myStdout.str());
			fs::WriteStringToFile(GetStdErrFile().string(), myStderr.str());
		}
		else
		{
			std::string expectedAst = fs::LoadFileToString(GetASTFile().string());
			std::string expectedOut = fs::LoadFileToString(GetStdOutFile().string());
			std::string expectedErr = fs::LoadFileToString(GetStdErrFile().string());

			EXPECT_EQ(ss.str(), expectedAst);
			EXPECT_EQ(myStdout.str(), expectedOut);
			EXPECT_EQ(myStderr.str(), expectedErr);
		}
	}

	boost::filesystem::path GetScriptFile() const { return (m_testDir / (m_regressionTestName + ".pgx")); }
	boost::filesystem::path GetASTFile() const { return (m_testDir / "ast.txt"); }
	boost::filesystem::path GetStdOutFile() const { return (m_testDir / "out.txt"); }
	boost::filesystem::path GetStdErrFile() const { return (m_testDir / "err.txt"); }

private:
	static boost::filesystem::path s_testFilesDirectory;
	static bool s_write;
	std::string m_regressionTestName;
	boost::filesystem::path m_testDir;

	ast::ProgramPtr m_program;
	Interpreter m_interpreter;
};
boost::filesystem::path RegressionTest::s_testFilesDirectory = "";
bool RegressionTest::s_write = false;

#define REGRESSION_TEST(NAME, EXPECTED)                        \
	TEST(RegressionTestCase, NAME)                             \
	{                                                          \
		try                                                    \
		{                                                      \
			RegressionTest r(#NAME);                           \
			r.Match(EXPECTED);                                 \
		}                                                      \
		catch (pagoda::common::exception::Exception & e)       \
		{                                                      \
			LOG_ERROR("Exception caught while running test:"); \
			LOG_ERROR(e.What());                               \
			throw;                                             \
		}                                                      \
	}

REGRESSION_TEST(float_operations, std::make_shared<FloatValue>(2.5f))
REGRESSION_TEST(integer_operations, std::make_shared<Integer>(2))
REGRESSION_TEST(string_operations, std::make_shared<String>("abc123123"))

REGRESSION_TEST(vector, std::make_shared<Vector3>(Vec3F{1, 2, 3}))
REGRESSION_TEST(plane, std::make_shared<DynamicPlane>(Plane<float>::FromPointAndNormal({0, 0, 0}, {1, 0, 0})))

int main(int argc, char* argv[])
{
	RegressionTest::SetExecutablePath(argv[0]);
	RegressionTest::SetWriteOutput(true);
	::testing::InitGoogleTest(&argc, argv);

	auto returnVal = RUN_ALL_TESTS();

	pagoda::common::instrument::ConsoleProfilerLogger consoleLogger(
	    pagoda::common::instrument::ProfilerManager::Instance());
	consoleLogger.Log(20);

	return returnVal;
}
