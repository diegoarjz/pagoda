#ifndef PAGODA_COMMON_LOGGER_H_
#define PAGODA_COMMON_LOGGER_H_

#include <cstdio>
#include <cstdlib>
#include <list>
#include <memory>
#include <sstream>
#include <string>

namespace pagoda::common::debug
{
class Logger
{
	public:
	enum class TraceLogs
	{
		Core,
		Common,
		MathLib,
		GeometryCore,
		GeometryOperations,
		Parameter,
		ProceduralGraph,
		ProceduralGraphReader,
		ProceduralObjects,
		Max
	};

	enum class ConsoleOutput
	{
		StdOut,
		StdErr,
		None
	};

	static Logger *trace();
	static Logger *debug();
	static Logger *info();
	static Logger *warning();
	static Logger *error();
	static Logger *fatal();

	explicit Logger(const std::string &out_file);
	explicit Logger(const ConsoleOutput &console_output);
	Logger(const std::string &out_file, const ConsoleOutput &console_output);
	virtual ~Logger();

	void Log(const char *message, ...);
	void Log(const std::string &message);
	virtual void Write(const char *message);

	static void Shutdown();
	static bool IsTraceEnabled(const TraceLogs &trace);

	private:
	static std::unique_ptr<Logger> sTrace;
	static std::unique_ptr<Logger> sDebug;
	static std::unique_ptr<Logger> sInfo;
	static std::unique_ptr<Logger> sWarning;
	static std::unique_ptr<Logger> sError;
	static std::unique_ptr<Logger> sFatal;

	struct LogFile
	{
		std::string m_path;
		std::FILE *m_file;
		uint32_t m_referenceCount;
	};  // struct LogFile
	static std::list<LogFile> sLogFiles;

	static std::FILE *RequestFile(const std::string &path);
	static void ReleaseFile(std::FILE *file);

	static const bool trace_enabled[static_cast<uint32_t>(TraceLogs::Max)];

	std::FILE *m_file;
	bool m_enabled;
	ConsoleOutput m_console;

};  // class Logger

#define LOG_TRACE(TRACE, message)                                                                       \
	if (pagoda::common::debug::Logger::IsTraceEnabled(pagoda::common::debug::Logger::TraceLogs::TRACE)) { \
		std::stringstream _logger_ss_;                                                                      \
		_logger_ss_ << #TRACE ": " << message;                                                              \
		pagoda::common::debug::Logger::trace()->Log(_logger_ss_.str());                                     \
	}

#define LOG_DEBUG(message)                                          \
	{                                                                 \
		std::stringstream _logger_ss_;                                  \
		_logger_ss_ << "Debug: " << message;                            \
		pagoda::common::debug::Logger::debug()->Log(_logger_ss_.str()); \
	}
#define LOG_INFO(message)                                           \
	{                                                                 \
		std::stringstream _logger_ss_;                                  \
		_logger_ss_ << "Info: " << message;                             \
		pagoda::common::debug::Logger::debug()->Log(_logger_ss_.str()); \
	}
#define LOG_WARNING(message)                                        \
	{                                                                 \
		std::stringstream _logger_ss_;                                  \
		_logger_ss_ << "Warning: " << message;                          \
		pagoda::common::debug::Logger::debug()->Log(_logger_ss_.str()); \
	}
#define LOG_ERROR(message)                                          \
	{                                                                 \
		std::stringstream _logger_ss_;                                  \
		_logger_ss_ << "Error: " << message;                            \
		pagoda::common::debug::Logger::debug()->Log(_logger_ss_.str()); \
	}
#define LOG_FATAL(message)                                          \
	{                                                                 \
		std::stringstream _logger_ss_;                                  \
		_logger_ss_ << "Fatal: " << message;                            \
		pagoda::common::debug::Logger::debug()->Log(_logger_ss_.str()); \
	}

}  // namespace pagoda::common::debug
#endif
