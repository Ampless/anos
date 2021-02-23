#include "gpu.h"
#include "mbox.h"
#include "uart0.h"
#include "image.h"

uint32_t &GPU::pixel(uint32_t x, uint32_t y) noexcept {
        return *(uint32_t*)&buffer[y*pitch+x*4];
}

void GPU::drawpixel(int x, int y, int r, int g, int b) noexcept {
        uint8_t *p = &buffer[y*pitch+x*4];
        p[0] = r;
        p[1] = g;
        p[2] = b;
}

GPU::GPU(uint32_t width, uint32_t height) noexcept {
        _Valid = false;

        mbox[0] = 35*4;
        mbox[1] = MBOX_REQUEST;

        mbox[2] = 0x48003; //set physical width & height
        mbox[3] = 8;
        mbox[4] = 8;
        mbox[5] = width; //w
        mbox[6] = height; //h

        mbox[7] = 0x48004; //set virtual width & height
        mbox[8] = 8;
        mbox[9] = 8;
        mbox[10] = width; //w
        mbox[11] = height; //h

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
                mbox[28] = mbox[28] & 0x3FFFFFFF; //GPU has different addrs, idk
                this->width  = mbox[5];
                this->height = mbox[6];
                pitch = mbox[33]; //number of bytes per line
                if(!mbox[24]) return; //BGR is an error.
                buffer=(uint8_t *)((uintptr_t)mbox[28]);
                _Valid = true;
        } else uart_puts("Can't set screen res (TODO: print res)\n");
}

bool GPU::valid() noexcept {
        return _Valid;
}

void GPU::showdemopicture() noexcept {
        uint32_t x,y;
        uint8_t *ptr=buffer;
        const char *data=header_data;
        char px[4];
        uint32_t h = height;
        uint32_t w = width;
        uint32_t p = pitch;
        uint32_t iw = image_width;
        uint32_t ih = image_height;

        ptr += (h - ih) / 2 * p + (w - iw) * 2;
        for(y=0;y<h;y++) {
                for(x=0;x<w;x++) {
                        HEADER_PIXEL(data, px);
                        pixel(x, y) = *((uint32_t *)&px);
                        ptr+=4;
                }
                ptr+=p-w*4;
        }
}
