#pragma once
#include <stdint.h>

// TODO: read a lot of docs and figure out wheather the isb is actually needed
#define mrs_to_var(reg, var) asm volatile("isb; mrs %0, " reg : "=r"(var))

class MemoryModel {
private:
        uint64_t mmfr0, mmfr1;

public:
        inline MemoryModel() noexcept {
                mrs_to_var("ID_AA64MMFR0_EL1", mmfr0);
                mrs_to_var("ID_AA64MMFR1_EL1", mmfr1);
        }

        // https://developer.arm.com/documentation/ddi0595/2020-12/AArch64-Registers/ID-AA64MMFR0-EL1--AArch64-Memory-Model-Feature-Register-0

        constexpr inline bool ecv() noexcept { return (mmfr0 >> 60) & 3; }
        constexpr inline bool fgt() noexcept { return ((mmfr0 >> 56) & 0xf) == 1; }
        constexpr inline bool exs() noexcept { return ((mmfr0 >> 44) & 0xf) == 1; }

        constexpr inline bool stage1_tgran4() noexcept {
                return ((mmfr0 >> 28) & 0xe) == 0;
        }
        constexpr inline bool stage1_tgran16() noexcept {
                uint32_t i = (mmfr0 >> 20) & 0xf;
                return i == 1 || i == 2;
        }
        constexpr inline bool stage1_tgran64() noexcept {
                return ((mmfr0 >> 24) & 0xf) == 0;
        }

        constexpr inline bool stage2_tgran4() noexcept {
                uint32_t i = (mmfr0 >> 40) & 0xf;
                if(i == 0) return stage1_tgran4();
                return (i & 2) == 2;
        }
        constexpr inline bool stage2_tgran16() noexcept {
                uint32_t i = (mmfr0 >> 32) & 0xf;
                if(i == 0) return stage1_tgran16();
                return (i & 2) == 2;
        }
        constexpr inline bool stage2_tgran64() noexcept {
                uint32_t i = (mmfr0 >> 36) & 0xf;
                if(i == 0) return stage1_tgran64();
                return i == 2;
        }

        constexpr inline bool mixed_endian() noexcept {
                return ((mmfr0 >> 8) & 0xf) == 1;
        }
        constexpr inline bool sns_mem() noexcept { return ((mmfr0 >> 12) & 0xf) == 1; }
        constexpr inline bool mixed_endian_el0() noexcept {
                return mixed_endian() || ((mmfr0 >> 16) & 0xf) == 1;
        }
        constexpr inline int32_t asid_bits() noexcept {
                uint32_t i = (mmfr0 >> 4) & 0xf;
                if(i == 0) return 8;
                if(i == 2) return 16;
                return -1;
        }
        constexpr inline int32_t pa_range() noexcept {
                switch(mmfr0 & 0xf) {
                        case 0: return 32;
                        case 1: return 36;
                        case 2: return 40;
                        case 3: return 42;
                        case 4: return 44;
                        case 5: return 48;
                        case 6: return 52;
                        default: return -1;
                }
        }

        // TODO: other registers
};

namespace {
static inline uint64_t rdtsc() {
        uint64_t i;
        mrs_to_var("CNTVCT_EL0", i);
        return i;
}

// NOTE: this is not always accurate
static inline uint32_t cpufrequency() {
        uint64_t i;
        mrs_to_var("CNTFRQ_EL0", i);
        return i & 0xffffffff;
}
}
