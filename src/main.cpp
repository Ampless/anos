#include "kinclude.h"
//#include "sft/schrift.h"

extern "C" void kmain(uint64_t dtb_ptr32,
                      uint64_t x1,
                      uint64_t x2,
                      uint64_t x3) {
        uint64_t start = get_system_timer();
        uart_init();

        // get the serial number with a mailbox call
        mbox[0] = 8*4;                  // length of the message
        mbox[1] = MBOX_REQUEST;

        mbox[2] = MBOX_TAG_GETSERIAL;   // get serial number command
        mbox[3] = 8;                    // buffer size
        mbox[4] = 8;
        mbox[5] = 0;                    // clear output buffer
        mbox[6] = 0;

        mbox[7] = MBOX_TAG_LAST;

        // send the message to the GPU and receive answer
        if (mbox_call(MBOX_CH_PROP))
                printf("My serial number is: "
                       "%x%x\n", mbox[6], mbox[5]);
        else uart_puts("Unable to query serial!\n");

        srand();
        printf("Random number: %8x\n", rand());

        //i really want to get rid of this, but it seems like i forgot
        //to wait for sth in gpu_init, because it segfaults in qemu
        usleep(1000000);

        GPU gpu;
        if(!gpu.valid()) {
                uart_puts("GPU wasn't initialized properly.\n");
                spinwhile(1);
        }
        gpu.showpicture();

        for(uint32_t x = 0; x < gpu.width; x++)
                for(uint32_t y = 0; y < gpu.height; y++)
                        gpu.drawpixel(x, y, x, y, rand());

        uint64_t end = get_system_timer();

        uart_puts("All of this took ");
        hex(end - start - 1000000);
        uart_puts(" microseconds.\n");

        uint32_t ass = x1 & 0xf;
        uart_puts("physical address space: ");
        uart_puts(ass == 0 ? "32" :
                  ass == 1 ? "36" :
                  ass == 2 ? "40" :
                  ass == 3 ? "42" :
                  ass == 4 ? "44" :
                  ass == 5 ? "48" :
                  ass == 6 ? "52" : "unknown");
        uart_putc('\n');

        x1 >>= 20;
        printf("16k granules %ssupported.\n", x1 & 0xf ? "" : "not ");
        x1 >>= 4;
        printf("64k granules %ssupported.\n", x1 & 0xf ? "not " : "");
        x1 >>= 4;
        printf("4k granules %ssupported.\n", x1 & 0xf ? "not " : "");

        while(1) uart_putc(uart_getc());
}
