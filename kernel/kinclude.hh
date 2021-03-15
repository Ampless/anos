#pragma once

#include <stdint.h>
#include <kalloc.hh>
#include <assert.hh>
#include <mem.hh>

#ifdef ANOS_ARM64_RASPI
# include <raspi/raspi.hh>
#endif

#ifdef ANOS_X86_PC
# include <pc/pc.hh>
#endif

#include <printf.hh>
