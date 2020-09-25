#pragma once
#include "../stdint.h"
#include "../mem.h"
#include "mmio.h"

#define spinwhile(b) while(b) cpu_relax()

#define SYSTMR_LO mem(MMIO_BASE+0x00003004)
#define SYSTMR_HI mem(MMIO_BASE+0x00003008)

namespace {
        inline void cpu_relax() {
                // this is 1:1 the code a small not big nor
                // professional (unlike gnu) project called linux uses
                // (i have actually looked up what arm says, yield
                //  is for, and using it seems fine to me)
                asm volatile("yield" ::: "memory");
        }
        void spincycles(int n) {
                if(n) spinwhile(n--);
        }
        inline volatile uint64_t get_system_timer()
        {
                return ((uint64_t) SYSTMR_HI << 32) | SYSTMR_LO;
        }
        inline volatile void usleep(uint64_t n)
        {
                uint64_t t = get_system_timer();
                if(!t) return;
                t += n;
                spinwhile(get_system_timer() < t);
        }
        inline volatile uint64_t rdtsc() {
                asm volatile ("isb; mrs x0, cntvct_el0");
        }
        // note: this is not always accurate
        inline volatile uint64_t cpufrequency() {
                asm volatile ("isb; mrs x0, cntfrq_el0");
        }
}
