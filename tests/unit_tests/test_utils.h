#ifndef PAGODA_TEST_TEST_UTILS_H_
#define PAGODA_TEST_TEST_UTILS_H_

#include <pagoda/common/debug/assertions.h>

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

pagoda::common::debug::Fail::FailBehaviour AssertExpected(const char *condition, const char *file, const int line,
                                                          const char *message);
struct ExpectAssert
{
	ExpectAssert();
	~ExpectAssert();
	static bool Asserted();
};

class MatchFile
{
	public:
	MatchFile(const boost::filesystem::path &filePath, bool saveFile = false);

	bool Match(const std::string &in);

	private:
	std::string m_fileContents;
	boost::filesystem::path m_filePath;
	bool m_saveFile;
};

class PagodaTestFixtureBase
{
	public:
	static void SetExecutablePath(const std::string &path);
	static boost::filesystem::path GetExecutablePath();
	static boost::filesystem::path GetExecutableDirectory();
	static void SetShouldWriteFiles(const bool &write);
	static bool GetShouldWriteFiles();

	private:
	static boost::filesystem::path s_executablePath;
	static boost::filesystem::path s_executableDirectory;
	static bool s_writeFiles;
};

template<class F>
class PagodaTestFixture : public PagodaTestFixtureBase, public F
{
	public:
	boost::filesystem::path GetTestFilesDir() const
	{
		return PagodaTestFixtureBase::GetExecutableDirectory() /= "test_files";
	}

	boost::filesystem::path GetCurrentTestFileInputDirectory()
	{
		boost::filesystem::path directory = GetTestFilesDir();
		directory /= ::testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
		directory /= ::testing::UnitTest::GetInstance()->current_test_info()->name();
		directory /= "input";
		return directory;
	}

	boost::filesystem::path GetCurrentTestFileResultsDirectory()
	{
		boost::filesystem::path directory = GetTestFilesDir();
		directory /= ::testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
		directory /= ::testing::UnitTest::GetInstance()->current_test_info()->name();
		directory /= "results";
		return directory;
	}
};

#endif
