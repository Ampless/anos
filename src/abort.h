#pragma once

#ifdef ANOS_ARM64_RASPI
# include "raspi/timing_and_power.h"
#endif

#ifdef ANOS_X86_PC
# include "pc/timing_and_power.h"
#endif
