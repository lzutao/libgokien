#ifndef GOKIEN_COMMON_H
#define GOKIEN_COMMON_H

/* C99 headers that Visual C++ does not support. */
#if defined(_MSC_VER) && (_MSC_VER < 1600)
    typedef __int8 int8_t;
    typedef unsigned __int8 uint8_t;
    typedef __int16 int16_t;
    typedef unsigned __int16 uint16_t;
    typedef __int32 int32_t;
    typedef unsigned __int32 uint32_t;
    typedef __int64 int64_t;
    typedef unsigned __int64 uint64_t;

    typedef enum { false, true } bool;
#else
#    include <stdint.h>
#    include <stdbool.h>
#endif

/* Those #define with empty ending is not used. */
#ifdef __cplusplus
#    define GOKIEN_BEGIN_DECL extern "C" {
#    define GOKIEN_END_DECL }
#else
#    define GOKIEN_BEGIN_DECL
#    define GOKIEN_END_DECL
#endif

/* See more about Visibility in https://gcc.gnu.org/wiki/Visibility */
#if defined _WIN32 || defined __CYGWIN__
#    ifdef GOKIEN_DLLEXPORT
#        define GOKIEN_EXTERN __declspec(dllexport)
#    else
#        define GOKIEN_EXTERN __declspec(dllimport)
#    endif
#    define GOKIEN_INTERN
#elif __GNUC__ >= 4
#    define GOKIEN_EXTERN extern __attribute__((visibility("default")))
#    define GOKIEN_INTERN extern __attribute__((visibility("hidden")))
#else
#    define GOKIEN_EXTERN
#    define GOKIEN_INTERN
#endif

#endif /* GOKIEN_COMMON_H */
