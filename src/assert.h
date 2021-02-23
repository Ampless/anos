#pragma once

#ifdef ANOS_NODEBUG
# define assert(expr)
#else
#include "printf.h"
namespace {
        void __assert_fail(const char *assertion, const char *file,
                           unsigned int line, const char *function) {
                printf("%s%s", __progname, *__progname ? ": " : "");
                printf("%s:%d: ", file, line);
                printf("%s", function ? function : "");
                printf("%s", function ? ": " : "");
                printf("Assertion '%s' failed.\n", assertion);
                abort();
        }
}

#define assert(expr) (static_cast<bool> (expr) ? (void)(0) \
                : __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))

// __PRETTY_FUNCTION__ is a GNU extension, which is a bit nicer than
// C's __func__.
# define __ASSERT_FUNCTION __extension__ __PRETTY_FUNCTION__

#endif
