#include "test_utils.h"

#include <common/file_util.h>

bool g_asserted = false;
selector::Fail::FailBehaviour AssertExpected(const char *condition, const char *file, const int line,
                                             const char *message)
{
	g_asserted = true;
	return selector::Fail::FailBehaviour::Continue;
}

ExpectAssert::ExpectAssert()
{
	g_asserted = false;
	selector::Fail::SetFailHandler(AssertExpected);
}

ExpectAssert::~ExpectAssert()
{
	g_asserted = false;
	selector::Fail::SetFailHandler(selector::DefaultHandler);
}

bool ExpectAssert::Asserted() { return g_asserted; }

MatchFile::MatchFile(const boost::filesystem::path &filePath, bool saveFile)
    : m_filePath(filePath), m_saveFile(saveFile)
{
	try
	{
		m_fileContents = selector::file_util::LoadFileToString(filePath.string());
	}
	catch (...)
	{
		EXPECT_TRUE(false) << "Could not open file " << filePath.string();
	}
}

bool MatchFile::Match(const std::string &in)
{
	if (m_saveFile)
	{
		selector::file_util::WriteStringToFile(m_filePath.string(), in);
		m_fileContents = in;
	}

	EXPECT_EQ(m_fileContents, in);
	return m_fileContents == in;
}

void SelectorTestFixtureBase::SetExecutablePath(const std::string &path)
{
	s_executablePath = path;
	s_executableDirectory = s_executablePath.remove_filename();
}

boost::filesystem::path SelectorTestFixtureBase::GetExecutablePath() { return s_executablePath; }

boost::filesystem::path SelectorTestFixtureBase::GetExecutableDirectory() { return s_executableDirectory; }

void SelectorTestFixtureBase::SetShouldWriteFiles(const bool &write) { s_writeFiles = write; }
bool SelectorTestFixtureBase::GetShouldWriteFiles() { return s_writeFiles; }

boost::filesystem::path SelectorTestFixtureBase::s_executablePath = "";
boost::filesystem::path SelectorTestFixtureBase::s_executableDirectory = "";
bool SelectorTestFixtureBase::s_writeFiles = false;
