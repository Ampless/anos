#pragma once
#include "mbox.h"
#include "timing.h"

#define PM_RSTC         mem(MMIO_BASE+0x0010001c)
#define PM_RSTS         mem(MMIO_BASE+0x00100020)
#define PM_WDOG         mem(MMIO_BASE+0x00100024)
#define PM_WDOG_MAGIC   0x5a000000
#define PM_RSTC_FULLRST 0x00000020

namespace {
        void shutdown(bool restart) {
                if(!restart) {
                        for(uint32_t i=0;i<16;i++) {
                                mbox[0]=8*4;
                                mbox[1]=MBOX_REQUEST;
                                mbox[2]=MBOX_TAG_SETPOWER;
                                mbox[3]=8;
                                mbox[4]=8;
                                mbox[5]=i;
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

                uint64_t r = PM_RSTS & ~0xfffffaaa;
                if(!restart) r |= 0x555;
                PM_RSTS = PM_WDOG_MAGIC | r;
                PM_WDOG = PM_WDOG_MAGIC | 10;
                PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
        }
}
