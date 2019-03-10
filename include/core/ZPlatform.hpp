#pragma once


/****** Detect Platform ******/
#if defined(_WIN32)
    #define PLATFORM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
    #if TARGET_OS_IPHONE
        #error iPhone is not supported
    #else
        #define PLATFORM_MACOS
    #endif
#elif defined(__linux__)
    #define PLATFORM_LINUX
#else
    #error Unsupported platform
#endif



/****** Detect Compiler ******/
#if defined(__clang__)
    #define COMPILER_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define COMPILER_GNU
#elif defined(_MSC_VER)
    #define COMPILER_MSC
#elif defined(__MINGW32__)
    #define COMPILER_MINGW
#elif defined(__CYGWIN__)
    #define COMPILER_CYGWIN
#endif
