#pragma once

#include "stdint.h"

#include "klib.h"

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


#define RNG_CTRL        mem(MMIO_BASE+0x00104000)
#define RNG_STATUS      mem(MMIO_BASE+0x00104004)
#define RNG_DATA        mem(MMIO_BASE+0x00104008)
#define RNG_INT_MASK    mem(MMIO_BASE+0x00104010)

#define SYSTMR_LO mem(MMIO_BASE+0x00003004)
#define SYSTMR_HI mem(MMIO_BASE+0x00003008)

#define PM_RSTC         mem(MMIO_BASE+0x0010001c)
#define PM_RSTS         mem(MMIO_BASE+0x00100020)
#define PM_WDOG         mem(MMIO_BASE+0x00100024)
#define PM_WDOG_MAGIC   0x5a000000
#define PM_RSTC_FULLRST 0x00000020

namespace {
        void srand() {
                RNG_STATUS = 0x40000;
                RNG_INT_MASK |= 1;
                RNG_CTRL |= 1;
                spinwhile(!(RNG_STATUS >> 24));
        }

        int rand() {
                return RNG_DATA;
        }

        int rand(int min, int max) {
                return rand() % (max - min) + min;
        }

        void spincycles(int n) {
                if(n) spinwhile(n--);
        }

        volatile uint64_t get_system_timer()
        {
                uint32_t h=SYSTMR_HI;
                uint32_t l=SYSTMR_LO;
                if(h!=SYSTMR_HI) {
                        h=SYSTMR_HI;
                        l=SYSTMR_LO;
                }
                return ((uint64_t) h << 32) | l;
        }

        void usleep(uint64_t n)
        {
                uint64_t t = get_system_timer();
                if(t) while(get_system_timer() < t+n);
        }

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
