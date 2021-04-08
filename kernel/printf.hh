#pragma once
#include <stdint.h>

// TODO: make portable
#include <raspi/uart0.hh>
#define putc uart_putc

uint32_t printf(const char *, ...);
uint32_t sprintf(char *, const char *, ...);
