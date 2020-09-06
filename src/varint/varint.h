#pragma once
#include "../stdint.h"

size_t int2varint(uint32_t i, uint8_t *bfr);
uint32_t varint2int(uint8_t *bfr);
size_t varintlen(uint8_t *bfr);
