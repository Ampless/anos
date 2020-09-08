#pragma once

#ifdef NDEBUG
# define assert(expr)
# define assert_perror(expr)
#else
#include "raspi/uart0.h"
namespace {
        void __assert_fail(const char *assertion, const char *file,
                        unsigned int line, const char *function) {
                //this should be printf,
                //but we currently dont have that
                uart_puts(__progname);
                uart_puts(*__progname ? ": " : "");
                uart_puts(file);
                uart_puts(":0x");
                hex(line);
                uart_puts(": ");
                uart_puts(function ? function : "");
                uart_puts(function ? ": " : "");
                uart_puts("Assertion '");
                uart_puts(assertion);
                uart_puts("' failed.\n");
                abort();
        }
}

# define assert(expr) (static_cast <bool> (expr) ? (void)(0) \
                : __assert_fail (#expr, __FILE__, __LINE__, __ASSERT_FUNCTION))

// __PRETTY_FUNCTION__ is a GNU extension, which is a bit nicer than
// C's __func__.
# define __ASSERT_FUNCTION __extension__ __PRETTY_FUNCTION__

#endif
