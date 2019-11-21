#include <gtest/gtest.h>

#include <selector.h>
#include <selscript/intermediate/ast.h>
#include <selscript/intermediate/ast_printer.h>
#include <selscript/interpreter/interpreter.h>
#include <selscript/parser/parser.h>

#include <dynamic_value/float_value.h>
#include <dynamic_value/integer_value.h>
#include <dynamic_value/string_value.h>
#include <dynamic_value/vector3.h>
#include <dynamic_value/dynamic_plane.h>

#include <math_lib/vec_base.h>

#include <common/exception.h>

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

	RegressionTest(const std::string& name, const std::string& code) : m_regressionTestName(name)
	{
		m_program = Parser().Parse(code);
        AstPrinter printer;
        m_program->AcceptVisitor(&printer);
	}

	template<class T>
	void Match(std::shared_ptr<T> expectedValue)
	{
		m_interpreter.Interpret(m_program);
		auto last = std::dynamic_pointer_cast<T>(m_interpreter.GetLastEvaluatedExpression());
		ASSERT_NE(last, nullptr);
		ASSERT_EQ(*last, *expectedValue);
	}

private:
	static boost::filesystem::path s_testFilesDirectory;
	std::string m_regressionTestName;

	ast::ProgramPtr m_program;
	Interpreter m_interpreter;
};
boost::filesystem::path RegressionTest::s_testFilesDirectory = "";

#define REGRESSION_TEST(NAME, CODE, EXPECTED)                                    \
	TEST(RegressionTestCase, NAME)                                               \
	{                                                                            \
		try                                                                      \
		{                                                                        \
			RegressionTest r(#NAME, CODE);                                       \
			r.Match(EXPECTED);                                                   \
		}                                                                        \
		catch (Exception & e)                                                    \
		{                                                                        \
			LOG_ERROR("Exception " << e.What() << " caught while running test"); \
			throw;                                                               \
		}                                                                        \
	}

REGRESSION_TEST(FloatOperations, "(1.0 + 2.0 * 2.0) / 2.0;", std::make_shared<FloatValue>(2.5f))
REGRESSION_TEST(IntegerOperations, "(1 + 2 * 2) / 2;", std::make_shared<Integer>(2))
REGRESSION_TEST(StringOperations, " \"abc\" + 2 * \"123\";", std::make_shared<String>("abc123123"))

REGRESSION_TEST(Vector3Creation, "Vector3(1,2,3);", std::make_shared<Vector3>(Vec3F(1, 2, 3)));
REGRESSION_TEST(PlaneCreation, "Plane(Vector3(1,2,3), Vector3(4,5,6));", std::make_shared<DynamicPlane>(Plane<float>::FromPointAndNormal({1,2,3}, {4,5,6})));

int main(int argc, char* argv[])
{
	RegressionTest::SetExecutablePath(argv[0]);
	::testing::InitGoogleTest(&argc, argv);

	auto returnVal = RUN_ALL_TESTS();

	ConsoleProfilerLogger consoleLogger(ProfilerManager::Instance());
	consoleLogger.Log(20);

	return returnVal;
}
