#pragma once


/****** Detect Platform ******/
#if defined(_WIN32)
    #define PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
    #define PLATFORM_MAC     1
#elif defined(__linux__)
    #define PLATFORM_LINUX   1
#endif



/****** Detect Compiler ******/
#if defined(__clang__)
    #define COMPILER_CLANG  1
#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_GNU    1
#elif defined(_MSC_VER)
    #define COMPILER_MSC    _MSC_VER
#elif defined(__MINGW32__)
    #define COMPILER_MINGW  1
#elif defined(__CYGWIN__)
    #define COMPILER_CYGWIN 1
#endif
