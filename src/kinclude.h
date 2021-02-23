#pragma once

#include "stdint.h"
#include "kalloc.h"
#include "klib.h"

#ifdef ANOS_ARM64_RASPI
# include "raspi/raspi.h"
#endif

#ifdef ANOS_X86_PC
# include "pc/pc.h"
#endif

#include "printf.h"
