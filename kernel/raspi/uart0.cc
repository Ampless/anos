#include "uart0.hh"

#include "../assert.hh"
#include "../mem.hh"
#include "mbox.hh"
#include "mmio.hh"
#include "timing_and_power.hh"

// PL011 UART registers
#define UART0_DR   mem(MMIO_BASE + 0x00201000)
#define UART0_FR   mem(MMIO_BASE + 0x00201018)
#define UART0_IBRD mem(MMIO_BASE + 0x00201024)
#define UART0_FBRD mem(MMIO_BASE + 0x00201028)
#define UART0_LCRH mem(MMIO_BASE + 0x0020102C)
#define UART0_CR   mem(MMIO_BASE + 0x00201030)
#define UART0_IMSC mem(MMIO_BASE + 0x00201038)
#define UART0_ICR  mem(MMIO_BASE + 0x00201044)

void uart_init() {
        int r;

        /* initialize UART */
        UART0_CR = 0; // turn off UART0

        /* set up clock for consistent divisor values */
        mbox[0] = 9 * 4;
        mbox[1] = MBOX_REQUEST;
        mbox[2] = MBOX_TAG_SETCLKRATE; // set clock rate
        mbox[3] = 12;
        mbox[4] = 8;
        mbox[5] = 2;       // UART clock
        mbox[6] = 4000000; // 4Mhz
        mbox[7] = 0;       // clear turbo
        mbox[8] = MBOX_TAG_LAST;
        mbox_call(MBOX_CH_PROP);

        /* map UART0 to GPIO pins */
        r = GPFSEL1;
        r &= ~((7 << 12) | (7 << 15)); // gpio14, gpio15
        r |= (4 << 12) | (4 << 15);    // alt0
        GPFSEL1 = r;
        GPPUD   = 0; // enable pins 14 and 15
        // weirdest timing hack ever TODO: cleanup
        spincycles(150);
        GPPUDCLK0 = (1 << 14) | (1 << 15);
        spincycles(150);
        GPPUDCLK0 = 0; // flush GPIO setup

        UART0_ICR  = 0x7FF; // clear interrupts
        UART0_IBRD = 2;     // 115200 baud
        UART0_FBRD = 0xB;
        UART0_LCRH = 0b11 << 5; // 8n1
        UART0_CR   = 0x301;     // enable Tx, Rx, FIFO
}

void uart_putc(int c) {
        if(!c) return;
        spinwhile(UART0_FR & 0x20);
        UART0_DR = c;
}

int uart_getc() {
        spinwhile(UART0_FR & 0x10);
        return UART0_DR;
}

void uart_puts(const char *s) {
        while(*s) uart_putc(*s++);
}
