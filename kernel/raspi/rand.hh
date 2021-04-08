#pragma once
#include <mem.hh>
#include <raspi/mmio.hh>

#define RNG_CTRL     mem(MMIO_BASE + 0x00104000)
#define RNG_STATUS   mem(MMIO_BASE + 0x00104004)
#define RNG_DATA     mem(MMIO_BASE + 0x00104008)
#define RNG_INT_MASK mem(MMIO_BASE + 0x00104010)
namespace {
// doesnt actually seed, but it just initializes
inline void srand() {
        RNG_STATUS   = 0x40000;
        RNG_INT_MASK = RNG_INT_MASK | 1;
        RNG_CTRL     = RNG_CTRL | 1;
        spinwhile(!(RNG_STATUS >> 24));
}

inline uint32_t rand() { return RNG_DATA; }

inline uint32_t rand(uint32_t min, uint32_t max) { return rand() % (max - min) + min; }
}
