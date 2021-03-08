#include <raspi/gpu.hh>
#include <raspi/mbox.hh>
#include <raspi/uart0.hh>

GPU::GPU(uint32_t width, uint32_t height) noexcept {
        this->_valid = false;

        uint32_t idx = 0;

        mbox[idx++] = 35*4;
        mbox[idx++] = MBOX_REQUEST;

        idx = mbox_cmd32(idx, 0x48003, 2, width, height); //set physical width & height
        idx = mbox_cmd32(idx, 0x48004, 2, width, height); //set virtual width & height
        idx = mbox_cmd32(idx, 0x48009, 2, 0, 0); //set virtual offset
        idx = mbox_cmd32(idx, 0x48005, 1, 32, 0); //set depth
        idx = mbox_cmd32(idx, 0x48006, 1, 1, 0); //set pixel order
        idx = mbox_cmd32(idx, 0x40001, 2, 4096, 0); //get framebuffer & alignment (pointer, size)
        idx = mbox_cmd32(idx, 0x40008, 1, 0, 0); //get pitch

        mbox[idx] = MBOX_TAG_LAST;

        if(mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28]) {
                this->_width  = mbox[5];
                this->_height = mbox[6];
                pitch = mbox[33]; //number of bytes per line
                //TODO: print err
                if(!mbox[24]) return; //BGR is an error.
                //                  GPU has different addrs, idk
                buffer=(uint8_t *)((uintptr_t)mbox[28] & 0x3FFFFFFF);
                this->_valid = true;
        } else uart_puts("Can't set screen res (TODO: print res)\n");
}

#include <raspi/demoimage/demoimage.h>
const char *dumb;

void GPU::showdemopicture() noexcept {
        draw_picture([](uint32_t x, uint32_t y) { return 0xffff4de1; });

        auto draw = [](uint32_t x, uint32_t y) {
                char px[4];
                HEADER_PIXEL(dumb, px);
                return *((uint32_t *)&px);
        };

        dumb = cmpt_data;
        draw_picture(draw, cmpt_width, cmpt_height, 262, 50);

        dumb = ludwig_data;
        draw_picture(draw, ludwig_width, ludwig_height, 112, 600);
}
