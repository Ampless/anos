#pragma once

#include "stdint.h"

#include "klib.h"

#include "raspi/timing.h"

namespace {
        inline volatile uint64_t rdtsc() {
                asm volatile ("isb; mrs x0, cntvct_el0");
        }
        // note: this is not always accurate
        inline volatile uint64_t cpufrequency() {
                asm volatile ("isb; mrs x0, cntfrq_el0");
        }
}

#include "raspi/mmio.h"

#include "raspi/mbox.h"

#include "raspi/uart0.h"

#include "raspi/rand.h"

#define PM_RSTC         mem(MMIO_BASE+0x0010001c)
#define PM_RSTS         mem(MMIO_BASE+0x00100020)
#define PM_WDOG         mem(MMIO_BASE+0x00100024)
#define PM_WDOG_MAGIC   0x5a000000
#define PM_RSTC_FULLRST 0x00000020

namespace {

        void shutdown(bool restart)
        {
                long r;

                if(!restart) {
                        for(r=0;r<16;r++) {
                                mbox[0]=8*4;
                                mbox[1]=MBOX_REQUEST;
                                mbox[2]=MBOX_TAG_SETPOWER;
                                mbox[3]=8;
                                mbox[4]=8;
                                mbox[5]=(int)r;
                                mbox[6]=0;
                                mbox[7]=MBOX_TAG_LAST;
                                mbox_call(MBOX_CH_PROP);
                        }

                        GPFSEL0 = 0;
                        GPFSEL1 = 0;
                        GPFSEL2 = 0;
                        GPFSEL3 = 0;
                        GPFSEL4 = 0;
                        GPFSEL5 = 0;
                        GPPUD = 0;
                        spincycles(150);
                        GPPUDCLK0 = 0xffffffff;
                        GPPUDCLK1 = 0xffffffff;
                        spincycles(150);
                        GPPUDCLK0 = 0;
                        GPPUDCLK1 = 0;
                }

                r = PM_RSTS;
                r &= ~0xfffffaaa;
                if(!restart) r |= 0x555;
                PM_RSTS = PM_WDOG_MAGIC | r;
                PM_WDOG = PM_WDOG_MAGIC | 10;
                PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
        }
}

#include "raspi/gpu.h"
