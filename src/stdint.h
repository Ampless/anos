#pragma once

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;

#if defined (__alpha__) || (defined (__sparc__) && defined(__arch64__)) || defined (__sparcv9)
typedef long int64_t;
typedef unsigned long uint64_t;
typedef long long int128_t;
typedef unsigned long long uint128_t;
#else
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

#if defined(__aarch64__) || defined(__amd64__)
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;
typedef uint64_t size_t;
#else
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
typedef uint32_t size_t;
#endif

#define NULL nullptr
