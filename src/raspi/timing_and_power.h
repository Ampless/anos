#pragma once
#include "../stdint.h"
#include "../mem.h"
#include "mmio.h"
#include "mbox.h"

#define spinwhile(b) while(b) cpu_relax()

#define SYSTMR_LO mem(MMIO_BASE+0x00003004)
#define SYSTMR_HI mem(MMIO_BASE+0x00003008)

#define PM_RSTC         mem(MMIO_BASE+0x0010001c)
#define PM_RSTS         mem(MMIO_BASE+0x00100020)
#define PM_WDOG         mem(MMIO_BASE+0x00100024)
#define PM_WDOG_MAGIC   0x5a000000
#define PM_RSTC_FULLRST 0x00000020

namespace {
        inline void cpu_relax() {
                // this is copy-paste from linux
                // (i have actually looked up what arm says, yield
                //  is for, and using it seems fine to me)
                asm volatile("yield" ::: "memory");
        }
        inline void spincycles(int n) {
                if(n) spinwhile(n--);
        }
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

namespace {
        //TODO: rename to clock
        inline uint64_t get_system_timer()
        {
                return ((uint64_t) SYSTMR_HI << 32) | SYSTMR_LO;
        }
        inline void usleep(uint64_t n)
        {
                uint64_t t = get_system_timer() + n;
                if(t == n) return;
                spinwhile(get_system_timer() < t);
        }
        inline uint64_t rdtsc() {
                asm volatile ("isb; mrs x0, cntvct_el0");
        }
        // note: this is not always accurate
        inline uint64_t cpufrequency() {
                asm volatile ("isb; mrs x0, cntfrq_el0");
        }
        inline void abort() {
                shutdown(false);
        }
}
