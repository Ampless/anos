#include "../src/rle/rle.c"
#include "../src/raspi/image.h"

extern void *malloc(unsigned long);
extern int putchar(int);

#define iw image_width
#define ih image_height
#define is iw * ih * 4

int main() {
        uint8_t *buffer = malloc(is * 2);
        size_t len = rlecompress(header_data, is, buffer);
        for(size_t i = 0; i < len; i++)
                putchar(buffer[i]);
}
