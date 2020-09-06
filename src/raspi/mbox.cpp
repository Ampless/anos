#include "mbox.h"

volatile uint32_t  __attribute__((aligned(16))) mbox[36];

int mbox_call(char ch)
{
    uint32_t r = (((uint32_t)((uintptr_t)&mbox)&~0xF) | (ch&0xF));
    spinwhile(MBOX_STATUS & MBOX_FULL);
    MBOX_WRITE = r;
    while(1) {
        spinwhile(MBOX_STATUS & MBOX_EMPTY);
        if(r == MBOX_READ) return mbox[1]==MBOX_RESPONSE;
    }
}
