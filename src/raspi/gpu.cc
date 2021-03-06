#include <raspi/gpu.hh>
#include <raspi/mbox.hh>
#include <raspi/uart0.hh>

GPU::GPU(uint32_t width, uint32_t height) noexcept {
        this->_valid = false;

        mbox[0] = 35*4;
        mbox[1] = MBOX_REQUEST;

        mbox[2] = 0x48003; //set physical width & height
        mbox[3] = 8;
        mbox[4] = 8;
        mbox[5] = width;
        mbox[6] = height;

        mbox[7] = 0x48004; //set virtual width & height
        mbox[8] = 8;
        mbox[9] = 8;
        mbox[10] = width;
        mbox[11] = height;

        mbox[12] = 0x48009; //set virtual offset
        mbox[13] = 8;
        mbox[14] = 8;
        mbox[15] = 0; //x
        mbox[16] = 0; //y

        mbox[17] = 0x48005; //set depth
        mbox[18] = 4;
        mbox[19] = 4;
        mbox[20] = 32; //bits

        mbox[21] = 0x48006; //set pixel order
        mbox[22] = 4;
        mbox[23] = 4;
        mbox[24] = 1; //RGB

        mbox[25] = 0x40001; //get framebuffer & alignment
        mbox[26] = 8;
        mbox[27] = 8;
        mbox[28] = 4096; //pointer
        mbox[29] = 0; //size

        mbox[30] = 0x40008; //get pitch
        mbox[31] = 4;
        mbox[32] = 4;
        mbox[33] = 0; //pitch

        mbox[34] = MBOX_TAG_LAST;

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
        draw_picture([](uint32_t x, uint32_t y) { return 0xff202020; });

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
