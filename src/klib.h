#pragma once
#include "stdint.h"
#include "raspi/timing.h"

namespace {
        inline void abort() {
                spinwhile(1);
        }
}

#define __progname "anOS"
#include "assert.h"

#include "mem.h"
