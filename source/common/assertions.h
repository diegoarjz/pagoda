#ifndef SELECTOR_COMMON_ASSERTIONS_H_
#define SELECTOR_COMMON_ASSERTIONS_H_

#include "utils.h"

#include <type_traits>

namespace selector
{
/**
 * Class that deals with the assertion infrastructure.
 * This class is completely static.
 */
class Fail
{
public:
	/**
	 * Fail behaviour for an assertion.
	 * This is returned by the FailHandler and its purpose is
	 * to indicate what should happen when an assertion is hit.
	 * Sometimes it is not desirable to halt execution (e.g. unit tests)
	 */
	enum class FailBehaviour
	{
		Halt,     ///< Halt execution and jump into debugger if possible
		Continue  ///< Continue execution
	};

	/**
	 * Type alias to a fail handler function.
	 * The implementation already provides a default one.
	 */
	using FailHandler = std::add_pointer<FailBehaviour(const char *condition, const char *file, const int line,
	                                                   const char *message)>::type;

	/**
	 * Installs a FailHandler function.
	 */
	static void SetFailHandler(const FailHandler &handler);
	/**
	 * Returns the current FailHandler.
	 */
	static FailHandler GetFailHandler();
	/**
	 * Used by the macros to report a fail.
	 * Should not be used directly.
	 */
	static FailBehaviour ReportFail(const char *condition, const char *file, const int line, const char *message, ...);

private:
	/// The currently installed handler
	static FailHandler sFailHandler;
};  // class Fail

typename Fail::FailBehaviour DefaultHandler(const char *condition, const char *file, const int line,
                                            const char *message);

}  // namespace selector

/// Interrupts the execution and tries to step into debugger
#define HALT asm("int $3")

#ifdef SELECTOR_ENABLE_ASSERTIONS

/**
 * Assertion that will only be called in DEBUG mode.
 */
#define DBG_ASSERT(cond)                                                                                             \
	do                                                                                                               \
	{                                                                                                                \
		if (!(cond))                                                                                                 \
		{                                                                                                            \
			if (::selector::Fail::ReportFail(#cond, __FILE__, __LINE__, 0) == ::selector::Fail::FailBehaviour::Halt) \
			{                                                                                                        \
				HALT;                                                                                                \
			}                                                                                                        \
		}                                                                                                            \
	} while (0)

/**
 * Assertion that will only be called in DEBUG mode.
 * Allows to print a custom message.
 */
#define DBG_ASSERT_MSG(cond, ...)                                                       \
	do                                                                                  \
	{                                                                                   \
		if (!(cond))                                                                    \
		{                                                                               \
			if (::selector::Fail::ReportFail(#cond, __FILE__, __LINE__, __VA_ARGS__) == \
			    ::selector::Fail::FailBehaviour::Halt)                                  \
			{                                                                           \
				HALT;                                                                   \
			}                                                                           \
		}                                                                               \
	} while (0)

#else

#define DBG_ASSERT(cond) \
	do                   \
	{                    \
		UNUSED(cond);    \
	} while (0)

#define DBG_ASSERT_MSG(cond, ...) \
	do                            \
	{                             \
		UNUSED(cond);             \
	} while (0)

#endif

/**
 * Assertion that will always be called.
 */
#define CRITICAL_ASSERT(cond)                                                              \
	do                                                                                     \
	{                                                                                      \
		if (!(cond))                                                                       \
		{                                                                                  \
			if (::selector::Fail::ReportFail("CRITICAL! " #cond, __FILE__, __LINE__, 0) == \
			    ::selector::Fail::FailBehaviour::Halt)                                     \
			{                                                                              \
				HALT;                                                                      \
			}                                                                              \
		}                                                                                  \
	} while (0)

/**
 * Assertion that will always be called.
 * Allows to print a custom message.
 */
#define CRITICAL_ASSERT_MSG(cond, ...)                                                               \
	do                                                                                               \
	{                                                                                                \
		if (!(cond))                                                                                 \
		{                                                                                            \
			if (::selector::Fail::ReportFail("CRITICAL! " #cond, __FILE__, __LINE__, __VA_ARGS__) == \
			    ::selector::Fail::FailBehaviour::Halt)                                               \
			{                                                                                        \
				HALT;                                                                                \
			}                                                                                        \
		}                                                                                            \
	} while (0)

#endif
