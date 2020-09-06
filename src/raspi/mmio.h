#pragma once

#include "config.h"

#if RASPI_TARGET == 3
# define MMIO_BASE 0x3F000000
#elif RASPI_TARGET == 4
# define MMIO_BASE 0xFE000000
#else
# error "Get yourself a RasPi 3/4!"
#endif

#undef RASPI_TARGET

#define GPFSEL0         mem(MMIO_BASE+0x200000)
#define GPFSEL1         mem(MMIO_BASE+0x200004)
#define GPFSEL2         mem(MMIO_BASE+0x200008)
#define GPFSEL3         mem(MMIO_BASE+0x20000C)
#define GPFSEL4         mem(MMIO_BASE+0x200010)
#define GPFSEL5         mem(MMIO_BASE+0x200014)
#define GPSET0          mem(MMIO_BASE+0x20001C)
#define GPSET1          mem(MMIO_BASE+0x200020)
#define GPCLR0          mem(MMIO_BASE+0x200028)
#define GPLEV0          mem(MMIO_BASE+0x200034)
#define GPLEV1          mem(MMIO_BASE+0x200038)
#define GPEDS0          mem(MMIO_BASE+0x200040)
#define GPEDS1          mem(MMIO_BASE+0x200044)
#define GPHEN0          mem(MMIO_BASE+0x200064)
#define GPHEN1          mem(MMIO_BASE+0x200068)
#define GPPUD           mem(MMIO_BASE+0x200094)
#define GPPUDCLK0       mem(MMIO_BASE+0x200098)
#define GPPUDCLK1       mem(MMIO_BASE+0x20009C)
