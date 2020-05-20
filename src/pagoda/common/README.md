# Common Library

These files should contain things like:

* File System
* Threads
* Debug system
* Profiling system
* Configuration Readers/Writers
* System abstractions
* Logging
* ...

## Assertion System

The main class of the assertions subsystem is the `pagoda::common::debug::Fail`. There are two fail behaviour modes,
specified in `Fail::FailBehaviour`, allowing to either halt the program (debug break) or print a message and continue.
The FailMode is returned from the FailHandler method.

It is possible to install different FailHandler with the `Fail::SetFailHandler` method. There is already a default
FailHandler that prints a message to the console.

To set a new FailHandler do:

```cpp
typename Fail::FailBehaviour DefaultHandler(const char *condition, const char *file, const int line,
                                            const char *message)
{
    // Print messages

    return Fail::FailBehaviour::Halt; // or Fail::FailBehaviour::Continue
}
Fail::SetFailHandler(NewHandler);
```

## Profiling System

The main class in the Profiler system is `pagoda::comon::debug::ProfilerManager`. It registers scoped profile data
where outer scopes don't account for time spent in inner scopes.

It is possible to profile different threads since the `ProfilerManager::Instance` method returns a thread local instance
of `ProfileManager`.

On construction, the `Profiler` class registers an entry by calling `ProfilerManager::EnterFunction()`. On desctruction
it calls the `Profiler::EndProfile()` method which, in turn calls the `ProfilerManager::ExitFunction()` measuring the
time spent between construction and the invocation to `Profiler::EndProfile()` (excluding nested scopes).

To use profiling within the code do

```cpp
void FunctionToProfile()
{
    START_PROFILE;

    {
        // Create a nested named profiler.
        // nested_profiler must be a valid c++ identifier.
        // Be careful with name clashes with other variables.
        START_NAMED_PROFILE(nested_profiler);

        // End the named profiler
        END_NAMED_PROFILE(nested_profiler)
    }

    // no need to to anything here
}
```

## Statistics System
