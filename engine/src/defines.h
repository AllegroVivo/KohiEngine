#pragma once

typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned int UInt32;
typedef unsigned long long UInt64;

typedef signed char Int8;
typedef signed short Int16;
typedef signed int Int32;
typedef signed long long Int64;

typedef float Single;
typedef double Double;

typedef _Bool Boolean;
typedef int Bool32;

#if defined(__clang__) || defined(__gcc__)
    #define STATIC_ASSERT _Static_assert
#else
    #define STATIC_ASSERT static_assert
#endif

STATIC_ASSERT(sizeof(UInt8) == 1, "Expected UInt8 to be 1 byte.");
STATIC_ASSERT(sizeof(UInt16) == 2, "Expected UInt16 to be 2 bytes.");
STATIC_ASSERT(sizeof(UInt32) == 4, "Expected UInt32 to be 4 bytes.");
STATIC_ASSERT(sizeof(UInt64) == 8, "Expected UInt64 to be 8 bytes.");

STATIC_ASSERT(sizeof(Int8) == 1, "Expected Int8 to be 1 byte.");
STATIC_ASSERT(sizeof(Int16) == 2, "Expected Int16 to be 2 bytes.");
STATIC_ASSERT(sizeof(Int32) == 4, "Expected Int32 to be 4 bytes.");
STATIC_ASSERT(sizeof(Int64) == 8, "Expected Int64 to be 8 bytes.");

STATIC_ASSERT(sizeof(Single) == 4, "Expected Single to be 4 bytes.");
STATIC_ASSERT(sizeof(Double) == 8, "Expected Double to be 8 bytes.");

#define TRUE 1
#define FALSE 0

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define KPLATFORM_WINDOWS 1
    #ifndef _WIN64
        #error "64-but is required on Windows!"
    #endif
#elif defined(__linux__) || defines(__gnu_linux__)
    #define KPLATFORM_LINUX 1
    #if defined(__ANDROID__)
        #define KPLATFORM_ANDROID 1
    #endif
#elif defined(__unix__)
    #define KPLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
    #define KPLATFORM_POSIX 1
#elif __APPLE__
    #define KPLATFORM_APPLE 1
    #include "<TargetConditionals.h>"
    #if TARGET_IPHONE_SIMULATOR
        #define KPLATFORM_IOS 1
        #define KPLATFORM_IOS_SIMULATOR 1
    #elif TARGET_OS_IPHONE
        #define KPLATFORM_IOS 1
    #elif TARGET_OS_MAC
    #else
        #error "Unknown Apple platform!"
    #endif
#else
    #error "Unknown platform!"
#endif

#ifdef KEXPORT
    #ifdef _MSC_VER
        #define KAPI __declspec(dllexport)
    #else
        #define KAPI __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define KAPI __declspec(dllimport)
    #else
        #define KAPI
    #endif
#endif

#define KCLAMP(value, min, max) (value <= min) ? min : (value >= max) ? max : value;

#ifdef _MSC_VER
    #define KINLINE __forceinline
    #define KNOINLINE __declspec(noinline)
#else
    #define KINLINE static inline
    #define KNOINLINE
#endif