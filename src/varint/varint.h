#pragma once
#include "../stdint.h"

size_t int2varint(uint32_t i, uint8_t *bfr);
uint32_t varint2int(uint8_t *bfr);
size_t varintlen(uint8_t *bfr);

inline size_t int2varint(uint32_t i, uint8_t *bfr) {
        if((i & 0xffffff80) == 0) {
                *bfr = i;
                return 1;
        } else if((i & 0xffffc000) == 0) {
                bfr[0] = i | 0x80;
                bfr[1] = i >> 7;
                return 2;
        } else if((i & 0xffe00000) == 0) {
                bfr[0] = i | 0x80;
                bfr[1] = i >> 7 | 0x80;
                bfr[2] = i >> 14;
                return 3;
        } else if((i & 0xf0000000) == 0) {
                bfr[0] = i | 0x80;
                bfr[1] = i >> 7 | 0x80;
                bfr[2] = i >> 14 | 0x80;
                bfr[3] = i >> 21;
                return 4;
        } else return -1;
}

inline uint32_t varint2int(uint8_t *bfr) {
        uint32_t i = *bfr & 0x7f, j = 0;
        while(bfr[j] & 0x80) i |= (bfr[++j] & 0x7f) << j*7;
        return i;
}

inline size_t varintlen(uint8_t *bfr) {
        size_t i = 0;
        while(bfr[i++] & 0x80);
        return i;
}
