#pragma once
#include "stdint.h"
#include "raspi/timing.h"

#define abort() spinwhile(1)

#define __progname "anOS"
#include "assert.h"

#include "mem.h"
