#include "kinclude.h"

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
                       "%8x%8x\n", mbox[6], mbox[5]);
        else uart_puts("Unable to query serial!\n");

        srand();
        printf("Random number: %8x\n", rand());

        uint32_t ass = x1 & 0xf;
        uart_puts("physical address space: ");
        if(ass <= 6)
                printf("%d bits", 32 + (
                       ass == 0 ? 0 :  ass == 1 ? 4 :
                       ass == 2 ? 8 :  ass == 3 ? 10 :
                       ass == 4 ? 12 : ass == 5 ? 16 : 20));
        else uart_puts("unknown");
        uart_putc('\n');

        x1 >>= 20;
        printf("16k granules %ssupported.\n", x1 & 0xf ? "" : "not ");
        x1 >>= 4;
        printf("64k granules %ssupported.\n", x1 & 0xf ? "not " : "");
        x1 >>= 4;
        printf("4k granules %ssupported.\n", x1 & 0xf ? "not " : "");

        //i really want to get rid of this, but it seems like i forgot
        //to wait for sth in gpu ctor, because it segfaults in qemu
        usleep(1000000);

        GPU gpu;
        assert(gpu.valid());
        gpu.showdemopicture();

        for(uint32_t x = 0; x < gpu.width; x++)
                for(uint32_t y = 0; y < gpu.height; y++)
                        gpu.drawpixel(x, y, x|rand(), y|rand(), (x^y)|rand());

        uint64_t end = get_system_timer();

        uart_puts("All of this took ");
        printf("%d milliseconds.\n", (end - start) / 1000 - 1000);

        printf("Allocated at %lx, %lx", kalloc(100), kalloc(100));

        spinwhile(1);
}
