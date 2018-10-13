#ifndef NB_TYPES_H
#define NB_TYPES_H

#ifdef __cplusplus
    #define NB_CPP
#endif

// Type

#ifdef _LIB
    #define NB_LIB
#endif

#if defined(NB_EXE) && defined(NB_LIB)
    #error "NB_EXE and NB_LIB can not be defined together"
#endif

// Compiler

#ifdef _MSC_VER
    #define NB_MSVC
#endif

#ifdef __MINGW32_VERSION
    #define NB_MINGW
#endif

#ifdef __GNUC__
    #define NB_GCC
#endif

#ifdef __clang__
    #define NB_CLANG
#endif

#ifdef __IAR_SYSTEMS_ICC__
  #define NB_IAR
#endif

// Platform

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
    #define NB_WINDOWS
#endif

#ifdef __linux__
    #define NB_LINUX
#endif

#ifdef ANDROID
    #define NB_ANDROID
#endif

#ifdef EMBEDDED
    #define NB_EMBEDDED
#endif

#ifdef __APPLE__
    #define NB_OSX
#endif

#if !defined(NB_WINDOWS) && !defined(NB_LINUX) && !defined(NB_ANDROID) && !defined(NB_EMBEDDED) && !defined(NB_OSX)
    #error "Current platform is not supported"
#endif

// Processor

#if defined(_M_IX86) || defined(__i386__)
    #define NB_X86
#endif

#if defined(_M_X64) || defined(__x86_64__)
    #define NB_X64
#endif

#if defined(NB_X86) || defined(NB_X64)
    #define NB_X86_FAMILY
#endif

#if defined(_M_IA64)
    #define NB_IA64
#endif

#if defined(__POWERPC__) || defined(_POWER) || defined(_ARCH_PPC)
    #define NB_POWER_PC
#endif

#if defined(NB_POWER_PC)
    #define NB_POWER_PC_FAMILY
#endif

#if defined(_M_ARM) ||  defined(__arm__) || defined(__ARM_ARCH)
    #define NB_ARM
#endif

#if defined(__arm64) || defined(__aarch64__)
    #define NB_ARM64
#endif

#if defined(NB_ARM) || defined(NB_ARM64)
    #define NB_ARM_FAMILY
#endif

#if defined(NB_X64) || defined(NB_IA64) || defined(NB_ARM64)
    #define NB_64
#elif defined(NB_X86) || defined(NB_POWER_PC) || defined(NB_ARM) 
    #define NB_86
#endif

#if defined(NB_64) && defined(NB_NO_INT_64)
    #error "NB_64 and NB_NO_INT_64 can not be defined together"
#endif

#include <stdarg.h>
#include <stdint.h>

#ifdef NB_CPP
extern "C"
{
#endif

/* Variable type definitions */

typedef int8_t   NBInt8;
typedef uint8_t  NBUInt8;
typedef int16_t  NBInt16;
typedef uint16_t NBUInt16;
typedef int32_t  NBInt32;
typedef uint32_t NBUInt32;
#ifndef NB_NO_INT_64
    typedef uint64_t NBUInt64;
    typedef int64_t  NBInt64;
#endif

typedef unsigned char  NBByte;
typedef signed char    NBSByte;
typedef unsigned short NBUShort;
typedef signed short   NBShort;
typedef unsigned int   NBUInt;
typedef signed int     NBInt;

#ifndef NB_NO_INT_64
    typedef unsigned long long NBULong;
    typedef signed long long   NBLong;
#endif

typedef NBInt32 NBBool32;
typedef NBBool32 NBBool;

#define NBTrue  1
#define NBFalse 0

#ifndef NB_NO_FLOAT
    typedef float NBSingle;
    typedef double NBDouble;
    typedef NBSingle NBFloat;
#endif

#ifdef NB_64
    typedef NBULong NBSizeType;
    typedef NBLong NBSSizeType;
#else
    #ifdef NB_MSVC
        typedef __w64 NBUInt NBSizeType;
        typedef __w64 NBInt  NBSSizeType;
    #else
        typedef NBUInt NBSizeType;
        typedef NBInt  NBSSizeType;
    #endif
#endif

typedef char NBAChar;

/* Handle and pointer type definitions */

typedef void NBVoid;
typedef NBVoid * NBPointer;
#define NB_PTR_SIZE sizeof(NBPointer)

#ifndef NULL
    #define NULL ((NBVoid *)0)
#endif

typedef NBPointer NBHandle;

/* Function and calling conventions definitions */

    #if defined(NB_MSVC) && _MSC_VER >= 1200
    #define NB_INLINE __forceinline
    #define NB_NO_INLINE __declspec(noinline)
#elif defined(NB_MSVC)
    #define NB_INLINE __inline
    #define NB_NO_INLINE __declspec(noinline)
#elif defined(NB_GCC)
    #define NB_INLINE inline
    #define NB_NO_INLINE __attribute__((noinline))
#else
    #define NB_INLINE inline
    #define NB_NO_INLINE
#endif

#ifdef NB_CPP
    #if defined(NB_MSVC) && !defined(NB_ARM_FAMILY)
        #define NB_NO_THROW __declspec(nothrow)
    #elif defined(NB_GCC) || defined(NB_CLANG)
        #define NB_NO_THROW __attribute__ ((nothrow))
    #else
        #define NB_NO_THROW
    #endif
#else
    #define NB_NO_THROW
    #define NB_EXTERN_C
#endif

#ifdef NB_MSVC
    #define NB_NO_RETURN __declspec(noreturn)
#elif defined(NB_GCC)
    #define NB_NO_RETURN __attribute__((noreturn))
#else
    #define NB_NO_RETURN
#endif

#ifdef NB_WINDOWS
    #define NB_CALL_CONV __stdcall
#else
    #define NB_CALL_CONV
#endif

#define NB_API NB_NO_THROW NB_CALL_CONV
#define NB_CALLBACK NB_CALL_CONV *

/* Misc definitions */

typedef struct
{
    NBInt iMajor;
    NBInt iMinor;
    NBInt iBuild;
    NBInt iRevision;
} NBVersion;

#ifdef NB_CPP
}
#endif

#endif // !NB_TYPES_H
