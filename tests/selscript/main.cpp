#include <gtest/gtest.h>

#include <selector.h>
#include <selscript/intermediate/ast.h>
#include <selscript/intermediate/ast_printer.h>
#include <selscript/interpreter/interpreter.h>
#include <selscript/parser/parser.h>

#include <dynamic_value/dynamic_plane.h>
#include <dynamic_value/float_value.h>
#include <dynamic_value/integer_value.h>
#include <dynamic_value/string_value.h>
#include <dynamic_value/vector3.h>

#include <math_lib/vec_base.h>

#include <common/exception.h>
#include <common/file_util.h>

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

	static void SetWriteOutput(bool write) { s_write = write; }

	RegressionTest(const std::string& name) : m_regressionTestName(name)
	{
		m_testDir = GetTestFilesDirectory() / "test_files" / name;
		std::string code = FileUtil::LoadFileToString(GetScriptFile().string());
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
			FileUtil::WriteStringToFile(GetASTFile().string(), ss.str());
			FileUtil::WriteStringToFile(GetStdOutFile().string(), myStdout.str());
			FileUtil::WriteStringToFile(GetStdErrFile().string(), myStderr.str());
		}
		else
		{
			std::string expectedAst = FileUtil::LoadFileToString(GetASTFile().string());
			std::string expectedOut = FileUtil::LoadFileToString(GetStdOutFile().string());
			std::string expectedErr = FileUtil::LoadFileToString(GetStdErrFile().string());

			EXPECT_EQ(ss.str(), expectedAst);
			EXPECT_EQ(myStdout.str(), expectedOut);
			EXPECT_EQ(myStderr.str(), expectedErr);
		}
	}

	boost::filesystem::path GetScriptFile() const { return (m_testDir / (m_regressionTestName + ".sscript")); }
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
		catch (Exception & e)                                  \
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

	ConsoleProfilerLogger consoleLogger(ProfilerManager::Instance());
	consoleLogger.Log(20);

	return returnVal;
}
