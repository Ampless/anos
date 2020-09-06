#include "kinclude.h"

extern "C" void kmain()
{
        uint64_t start = rdtsc();
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

        uint64_t end = rdtsc();

        uart_puts("All of this took ");
        hex(end - start);
        uart_puts(" cycles.\n");

        srand();
        uart_puts("Random number: ");
        hex(rand());
        uart_putc('\n');

        uart_putc('3');
        wait_usec_st(1000000);
        uart_putc('2');
        wait_usec_st(1000000);
        uart_putc('1');
        wait_usec_st(1000000);
        uart_puts("0 done.");

        gpu_init();
        gpu_showpicture();

        for(int x = 100; x < 200; x++)
                for(int y = 100; y < 200; y++)
                        gpu_drawpixel(x, y, x, y, 0);

        while(1) uart_putc(uart_getc());
}
