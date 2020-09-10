#include "logger.h"

#include <stdarg.h>
#include <fstream>
#include <iostream>

namespace pagoda::common::debug
{
std::unique_ptr<Logger> Logger::sTrace = nullptr;
std::unique_ptr<Logger> Logger::sDebug = nullptr;
std::unique_ptr<Logger> Logger::sInfo = nullptr;
std::unique_ptr<Logger> Logger::sWarning = nullptr;
std::unique_ptr<Logger> Logger::sError = nullptr;
std::unique_ptr<Logger> Logger::sFatal = nullptr;
std::list<typename Logger::LogFile> Logger::sLogFiles;

const bool Logger::trace_enabled[static_cast<uint32_t>(Logger::TraceLogs::Max)] = {
  false,  // Core,
  false,  // Common,
  false,  // MathLib,
  false,  // GeometryCore,
  false,  // GeometryOperations,
  false,  // Parameter,
  false,  // ProceduralGraph,
  false,  // ProceduralGraphReader,
  false,  // ProceduralObjects,
};

Logger *Logger::trace()
{
	if (sTrace == nullptr) {
		sTrace = std::make_unique<Logger>("debug.log");
	}
	return sTrace.get();
}

Logger *Logger::debug()
{
	if (sDebug == nullptr) {
		sDebug = std::make_unique<Logger>("debug.log", ConsoleOutput::StdErr);
	}
	return sDebug.get();
}

Logger *Logger::info()
{
	if (sInfo == nullptr) {
		sInfo = std::make_unique<Logger>("pagoda.log");
	}
	return sInfo.get();
}

Logger *Logger::warning()
{
	if (sWarning == nullptr) {
		sWarning = std::make_unique<Logger>("pagoda.log", ConsoleOutput::StdOut);
	}
	return sWarning.get();
}

Logger *Logger::error()
{
	if (sError == nullptr) {
		sError = std::make_unique<Logger>("pagoda.log", ConsoleOutput::StdErr);
	}
	return sError.get();
}

Logger *Logger::fatal()
{
	if (sFatal == nullptr) {
		sFatal = std::make_unique<Logger>("pagoda.log", ConsoleOutput::StdErr);
	}
	return sFatal.get();
}

Logger::Logger(const std::string &out_file) : m_enabled(true), m_console(ConsoleOutput::None)
{
	m_file = RequestFile(out_file);

	if (!m_file) {
		m_enabled = false;
	}
}

Logger::Logger(const ConsoleOutput &console_output) : m_file(), m_enabled(true), m_console(console_output) {}

Logger::Logger(const std::string &out_file, const ConsoleOutput &console_output)
  : m_file(), m_enabled(true), m_console(console_output)
{
	m_file = RequestFile(out_file);

	if (!m_file) {
		m_enabled = false;
	}
}

Logger::~Logger()
{
	if (m_file) {
		ReleaseFile(m_file);
	}
}

void Logger::Log(const char *message, ...)
{
	if (!m_enabled) {
		return;
	}

	char final_message[1024];

	if (message != nullptr) {
		va_list args;
		va_start(args, message);
		vsnprintf(final_message, 1024, message, args);
		va_end(args);

		Write(final_message);
	}
}
void Logger::Log(const std::string &message)
{
	if (m_enabled) {
		Write(message.c_str());
	}
}

void Logger::Write(const char *message)
{
	if (m_file) {
		std::fprintf(m_file, "%s\n", message);
		std::fflush(m_file);
	}

	if (m_console == ConsoleOutput::StdOut) {
		std::cout << message << std::endl;
	} else if (m_console == ConsoleOutput::StdErr) {
		std::cerr << message << std::endl;
	}
}

std::FILE *Logger::RequestFile(const std::string &path)
{
	for (auto &f : sLogFiles) {
		if (f.m_path == path) {
			f.m_referenceCount++;
			return f.m_file;
		}
	}

	LogFile new_log_file;
	new_log_file.m_path = path;
	new_log_file.m_referenceCount = 1;
	new_log_file.m_file = std::fopen(path.c_str(), "w");

	if (new_log_file.m_file) {
		sLogFiles.push_back(new_log_file);
	}

	return new_log_file.m_file;
}

void Logger::ReleaseFile(std::FILE *file)
{
	std::list<std::list<LogFile>::iterator> filesToErase;

	for (auto iter = sLogFiles.begin(); iter != sLogFiles.end(); ++iter) {
		if (iter->m_file == file) {
			iter->m_referenceCount--;
			if (iter->m_referenceCount == 0) {
				std::fclose(iter->m_file);
				filesToErase.push_back(iter);
			}
		}
	}

	for (auto iter : filesToErase) {
		sLogFiles.erase(iter);
	}
}

void Logger::Shutdown()
{
	sTrace = nullptr;
	sDebug = nullptr;
	sInfo = nullptr;
	sWarning = nullptr;
	sError = nullptr;
	sFatal = nullptr;
}

bool Logger::IsTraceEnabled(const TraceLogs &trace) { return trace_enabled[static_cast<uint32_t>(trace)]; }

}  // namespace pagoda::common::debug
