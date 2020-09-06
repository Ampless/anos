#include "rle.h"
#include "../varint/varint.h"

size_t rlecompress(uint8_t *src, size_t srclen, uint8_t *dst) {
        if(srclen == 0) return 0;
        uint32_t current_char = *src, current_count = -1;
        uint8_t *dstorg = dst;
        for(size_t i = 0; i < srclen; i++) {
                if(current_char != src[i] || current_count == 0x7fffffff) {
                        uint8_t bfr[4];
                        int2varint(current_count, bfr);
                        for(int i = 0; i < varintlen(bfr); i++)
                                *dst++ = bfr[i];
                        *dst++ = current_char;
                        current_count = 0;
                        current_char = src[i];
                } else {
                        current_count++;
                }
        }
        return dst - dstorg;
}

size_t rledecompress(uint8_t *src, size_t srclen, uint8_t *dst) {
        if(srclen == 0 || srclen % 2) return 0;
        uint8_t *dstorg = dst;
        for(size_t i = 0; i < srclen; i++) {
                uint32_t len = varint2int(src);
                i += varintlen(src);
                for(uint32_t j = 0; j <= len; j++)
                        *dst++ = src[i];
        }
        return dst - dstorg;
}
