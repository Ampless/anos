#include "../stdint.h"

size_t rlecompress(uint8_t *src, size_t srclen, uint8_t *dst);
size_t rledecompress(uint8_t *src, size_t srclen, uint8_t *dst);
