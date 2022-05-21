#pragma once

// Define PAGODA_API for any platform
#ifdef PAGODA_SHARED_LIB
#if defined _WIN32 || defined __CYGWIN__
  #ifdef WIN_EXPORT
    // Exporting...
    #ifdef __GNUC__
      #define PAGODA_API __attribute__ ((dllexport))
    #else
      #define PAGODA_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define PAGODA_API __attribute__ ((dllimport))
    #else
      #define PAGODA_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define PAGODA_HIDDEN
#else
  #if __GNUC__ >= 4
    #define PAGODA_API __attribute__ ((visibility ("default")))
    #define PAGODA_HIDDEN  __attribute__ ((visibility ("hidden")))
  #else
    #define PAGODA_API
    #define PAGODA_HIDDEN
  #endif
#endif
#else
    #define PAGODA_API
    #define PAGODA_HIDDEN
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef WIN_EXPORT
    // Exporting...
    #ifdef __GNUC__
      #define PAGODA_PLUGIN __attribute__ ((dllexport))
    #else
      #define PAGODA_PLUGIN __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define PAGODA_PLUGIN __attribute__ ((dllimport))
    #else
      #define PAGODA_PLUGIN __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
#else
  #if __GNUC__ >= 4
    #define PAGODA_PLUGIN __attribute__ ((visibility ("default")))
  #else
    #define PAGODA_PLUGIN
  #endif
#endif
