#pragma once

#include <assert.hh>
#include <kalloc.hh>
#include <mem.hh>
#include <stdint.h>

#ifdef ANOS_ARM64_RASPI
#include <raspi/raspi.hh>
#endif

#ifdef ANOS_X86_PC
#include <pc/pc.hh>
#endif

#include <printf.hh>
