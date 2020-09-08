#pragma once
#include "stdint.h"

uint32_t printf(const char *, ...);
uint32_t sprintf(char *, const char *, ...);
uint32_t vprintf(const char *, __builtin_va_list);
uint32_t vsprintf(char *, const char *, __builtin_va_list);
uint32_t rprintf(void(*)(int), const char *, __builtin_va_list);
