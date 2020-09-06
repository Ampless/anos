#pragma once
#include "stdint.h"

#define spinwhile(b) while(b) cpu_relax()

namespace {
        inline void cpu_relax() {
                // this is 1:1 the code a small not big nor
                // professional (unlike gnu) project called linux uses
                // (i have actually looked up what arm says, yield
                //  is for and using it seems fine to me)
                asm volatile("yield" ::: "memory");
        }
        inline volatile uint32_t &mem(const uintptr_t loc) {
                return *reinterpret_cast<uint32_t *>(loc);
        }
}
