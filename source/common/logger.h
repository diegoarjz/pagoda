#ifndef SELECTOR_COMMON_LOGGER_H_
#define SELECTOR_COMMON_LOGGER_H_

#include <cstdio>
#include <cstdlib>
#include <list>
#include <memory>
#include <string>

namespace selector
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

#define LOG_TRACE(TRACE, message, ...)                                        \
	if (selector::Logger::IsTraceEnabled(selector::Logger::TraceLogs::TRACE)) \
	{                                                                         \
		selector::Logger::trace()->Log(#TRACE ": " message, ##__VA_ARGS__);   \
	}

#define LOG_DEBUG(message, ...) selector::Logger::debug()->Log("Debug: " message, ##__VA_ARGS__)
#define LOG_INFO(message, ...) selector::Logger::info()->Log("Info: " message, ##__VA_ARGS__)
#define LOG_WARNING(message, ...) selector::Logger::warning()->Log("Warning: " message, ##__VA_ARGS__)
#define LOG_ERROR(message, ...) selector::Logger::error()->Log("Error: " message, ##__VA_ARGS__)
#define LOG_FATAL(message, ...) selector::Logger::fatal()->Log("Fatal: " message, ##__VA_ARGS__)

}  // namespace selector
#endif
