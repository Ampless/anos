#include "kinclude.h"

extern "C" void kmain()
{
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
        if (mbox_call(MBOX_CH_PROP)) {
                uart_puts("My serial number is: ");
                hex(mbox[6]);
                hex(mbox[5]);
                uart_putc('\n');
        } else uart_puts("Unable to query serial!\n");

        srand();
        uart_puts("Random number: ");
        hex(rand());
        uart_putc('\n');

        //i really want to get rid of this, but it seems like i forgot
        //to wait for sth in gpu_init, because it segfaults in qemu
        usleep(1000000);

        gpu_init();
        gpu_showpicture();

        for(uint32_t x = 0; x < gpu.width; x++)
                for(uint32_t y = 0; y < gpu.height; y++)
                        gpu.pixel(x, y) = rand();

        uint64_t end = get_system_timer();

        uart_puts("All of this took ");
        hex(end - start - 1000000);
        uart_puts(" microseconds.\n");

        while(1) uart_putc(uart_getc());
}
