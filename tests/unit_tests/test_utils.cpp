#include "test_utils.h"

#include <common/file_util.h>

bool g_asserted = false;
pagoda::Fail::FailBehaviour AssertExpected(const char *condition, const char *file, const int line,
                                             const char *message)
{
	g_asserted = true;
	return pagoda::Fail::FailBehaviour::Continue;
}

ExpectAssert::ExpectAssert()
{
	g_asserted = false;
	pagoda::Fail::SetFailHandler(AssertExpected);
}

ExpectAssert::~ExpectAssert()
{
	g_asserted = false;
	pagoda::Fail::SetFailHandler(pagoda::DefaultHandler);
}

bool ExpectAssert::Asserted() { return g_asserted; }

MatchFile::MatchFile(const boost::filesystem::path &filePath, bool saveFile)
    : m_filePath(filePath), m_saveFile(saveFile)
{
	try
	{
		m_fileContents = pagoda::file_util::LoadFileToString(filePath.string());
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
		pagoda::file_util::CreateDirectories(m_filePath.parent_path());
		pagoda::file_util::WriteStringToFile(m_filePath.string(), in);
		m_fileContents = in;
		return true;
	}

	EXPECT_EQ(m_fileContents, in);
	return m_fileContents == in;
}

void PagodaTestFixtureBase::SetExecutablePath(const std::string &path)
{
	s_executablePath = path;
	s_executableDirectory = s_executablePath.remove_filename();
}

boost::filesystem::path PagodaTestFixtureBase::GetExecutablePath() { return s_executablePath; }

boost::filesystem::path PagodaTestFixtureBase::GetExecutableDirectory() { return s_executableDirectory; }

void PagodaTestFixtureBase::SetShouldWriteFiles(const bool &write) { s_writeFiles = write; }
bool PagodaTestFixtureBase::GetShouldWriteFiles() { return s_writeFiles; }

boost::filesystem::path PagodaTestFixtureBase::s_executablePath = "";
boost::filesystem::path PagodaTestFixtureBase::s_executableDirectory = "";
bool PagodaTestFixtureBase::s_writeFiles = false;
