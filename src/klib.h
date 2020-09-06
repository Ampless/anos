#pragma once
#include "stdint.h"

namespace {
        inline volatile uint32_t &mem(const uintptr_t loc) {
                return *reinterpret_cast<uint32_t *>(loc);
        }
}
