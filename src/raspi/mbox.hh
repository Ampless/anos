#pragma once
#include <raspi/mmio.hh>
#include <mem.hh>
#include <stdint.h>

extern volatile uint32_t mbox[36];

#define MBOX_REQUEST    0

#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

#define MBOX_TAG_GETSERIAL      0x10004
#define MBOX_TAG_SETCLKRATE     0x38002
#define MBOX_TAG_LAST           0

#define VIDEOCORE_MBOX  (MMIO_BASE+0x0000B880)
#define MBOX_READ       mem(VIDEOCORE_MBOX+0x0)
#define MBOX_POLL       mem(VIDEOCORE_MBOX+0x10)
#define MBOX_SENDER     mem(VIDEOCORE_MBOX+0x14)
#define MBOX_STATUS     mem(VIDEOCORE_MBOX+0x18)
#define MBOX_CONFIG     mem(VIDEOCORE_MBOX+0x1C)
#define MBOX_WRITE      mem(VIDEOCORE_MBOX+0x20)
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000
#define MBOX_TAG_SETPOWER       0x28001
#define MBOX_TAG_SETCLKRATE     0x38002
#define MBOX_TAG_LAST           0

bool mbox_call(const char channel);

constexpr static inline size_t mbox_cmd(size_t idx,
                                        const uint32_t cmd,
                                        const uint32_t len) {
        mbox[idx++] = cmd;
        mbox[idx++] = len;
        mbox[idx++] = len;
        return idx;
}

constexpr static inline size_t mbox_cmd32(size_t         idx,
                                          const uint32_t cmd,
                                          const uint32_t len,
                                          const uint32_t arg1,
                                          const uint32_t arg2) {
        //static_assert(len == 1 || len == 2);
        idx = mbox_cmd(idx, cmd, len * 4);
        mbox[idx++] = arg1;
        if(len == 2) mbox[idx++] = arg2;
        return idx;
}
