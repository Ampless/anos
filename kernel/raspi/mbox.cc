#include <raspi/mbox.hh>
#include <raspi/timing_and_power.hh>
#include <assert.hh>

volatile uint32_t __attribute__((aligned(16))) mbox[36];

bool mbox_call(const char ch) {
        //both of these should be static_assert, but cant
        assert(ch < 0x10);
        assert((((uintptr_t)&mbox) & 0xffffffff0000000f) == 0);
        uint32_t r = (((uint32_t)(uintptr_t)&mbox) | ch);
        spinwhile(MBOX_STATUS & MBOX_FULL);
        MBOX_WRITE = r;
        while(1) {
                spinwhile(MBOX_STATUS & MBOX_EMPTY);
                if(r == MBOX_READ) return mbox[1] == MBOX_RESPONSE;
        }
}
