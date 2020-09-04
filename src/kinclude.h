#pragma once

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;

#if defined (__alpha__) || (defined (__sparc__) && defined(__arch64__)) || defined (__sparcv9)
typedef long int64_t;
typedef unsigned long uint64_t;
typedef long long int128_t;
typedef unsigned long long uint128_t;
#else
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

#if defined(__aarch64__) || defined(__amd64__)
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;
#else
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
#endif

#define spinwhile(b) while(b) asm volatile("nop")

namespace {
        volatile uint32_t &mem(const uintptr_t loc) {
                return *reinterpret_cast<uint32_t *>(loc);
        }
        volatile uint64_t rdtsc() {
                uint64_t i;
                asm volatile ("isb; mrs %0, cntvct_el0" : "=r" (i));
                return i;
        }
        // note: this is not always accurate
        volatile uint64_t cpufrequency() {
                uint64_t i;
                asm volatile ("isb; mrs %0, cntfrq_el0" : "=r" (i));
                return i;
        }
}

#include "../config.h"
#if RASPI_TARGET == 3
# define MMIO_BASE 0x3F000000
#elif RASPI_TARGET == 4
# define MMIO_BASE 0xFE000000
#else
# error "Get yourself a RasPi 3/4!"
#endif

#define GPFSEL0         mem(MMIO_BASE+0x200000)
#define GPFSEL1         mem(MMIO_BASE+0x200004)
#define GPFSEL2         mem(MMIO_BASE+0x200008)
#define GPFSEL3         mem(MMIO_BASE+0x20000C)
#define GPFSEL4         mem(MMIO_BASE+0x200010)
#define GPFSEL5         mem(MMIO_BASE+0x200014)
#define GPSET0          mem(MMIO_BASE+0x20001C)
#define GPSET1          mem(MMIO_BASE+0x200020)
#define GPCLR0          mem(MMIO_BASE+0x200028)
#define GPLEV0          mem(MMIO_BASE+0x200034)
#define GPLEV1          mem(MMIO_BASE+0x200038)
#define GPEDS0          mem(MMIO_BASE+0x200040)
#define GPEDS1          mem(MMIO_BASE+0x200044)
#define GPHEN0          mem(MMIO_BASE+0x200064)
#define GPHEN1          mem(MMIO_BASE+0x200068)
#define GPPUD           mem(MMIO_BASE+0x200094)
#define GPPUDCLK0       mem(MMIO_BASE+0x200098)
#define GPPUDCLK1       mem(MMIO_BASE+0x20009C)

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

int mbox_call(char ch);

// PL011 UART registers
#define UART0_DR        mem(MMIO_BASE+0x00201000)
#define UART0_FR        mem(MMIO_BASE+0x00201018)
#define UART0_IBRD      mem(MMIO_BASE+0x00201024)
#define UART0_FBRD      mem(MMIO_BASE+0x00201028)
#define UART0_LCRH      mem(MMIO_BASE+0x0020102C)
#define UART0_CR        mem(MMIO_BASE+0x00201030)
#define UART0_IMSC      mem(MMIO_BASE+0x00201038)
#define UART0_ICR       mem(MMIO_BASE+0x00201044)

namespace {
        void uart_init()
        {
                int r;

                /* initialize UART */
                UART0_CR = 0;         // turn off UART0

                /* set up clock for consistent divisor values */
                mbox[0] = 9*4;
                mbox[1] = MBOX_REQUEST;
                mbox[2] = MBOX_TAG_SETCLKRATE; // set clock rate
                mbox[3] = 12;
                mbox[4] = 8;
                mbox[5] = 2;           // UART clock
                mbox[6] = 4000000;     // 4Mhz
                mbox[7] = 0;           // clear turbo
                mbox[8] = MBOX_TAG_LAST;
                mbox_call(MBOX_CH_PROP);

                /* map UART0 to GPIO pins */
                r=GPFSEL1;
                r&=~((7<<12)|(7<<15)); // gpio14, gpio15
                r|=(4<<12)|(4<<15);    // alt0
                GPFSEL1 = r;
                GPPUD = 0;            // enable pins 14 and 15
                r=150; while(r--) { asm volatile("nop"); }
                GPPUDCLK0 = (1<<14)|(1<<15);
                r=150; while(r--) { asm volatile("nop"); }
                GPPUDCLK0 = 0;        // flush GPIO setup

                UART0_ICR = 0x7FF;    // clear interrupts
                UART0_IBRD = 2;       // 115200 baud
                UART0_FBRD = 0xB;
                UART0_LCRH = 0b11<<5; // 8n1
                UART0_CR = 0x301;     // enable Tx, Rx, FIFO
        }

        void uart_putc(int c) {
                spinwhile(UART0_FR&0x20);
                UART0_DR=c;
        }

        int uart_getc() {
                spinwhile(UART0_FR&0x10);
                return UART0_DR;
        }

        void uart_puts(char *s) {
                while(*s) uart_putc(*s++);
        }

        /**
         * Display a binary value in hexadecimal
         * (really temporary)
         */
        void hex(int d) {
                int n, c;
                for(c=28;c>=0;c-=4) {
                        n=(d>>c)&0xF;
                        n+=n>9?0x37:0x30;
                        uart_putc(n);
                }
        }
}

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

#define RNG_CTRL        mem(MMIO_BASE+0x00104000)
#define RNG_STATUS      mem(MMIO_BASE+0x00104004)
#define RNG_DATA        mem(MMIO_BASE+0x00104008)
#define RNG_INT_MASK    mem(MMIO_BASE+0x00104010)

#define SYSTMR_LO mem(MMIO_BASE+0x00003004)
#define SYSTMR_HI mem(MMIO_BASE+0x00003008)

#define spincycles(n) if(n) spinwhile(n--);

namespace {
        void srand() {
                RNG_STATUS = 0x40000;
                RNG_INT_MASK |= 1;
                RNG_CTRL |= 1;
                spinwhile(!(RNG_STATUS >> 24));
        }

        int rand() {
                return RNG_DATA;
        }

        int rand(int min, int max) {
                return rand() % (max - min) + min;
        }

        volatile long get_system_timer()
        {
                uint32_t h=-1, l;
                h=SYSTMR_HI;
                l=SYSTMR_LO;
                if(h!=SYSTMR_HI) {
                        h=SYSTMR_HI;
                        l=SYSTMR_LO;
                }
                return ((uint64_t) h << 32) | l;
        }

        void wait_usec_st(int n)
        {
                long t = get_system_timer();
                if(t) while(get_system_timer() < t+n);
        }
}
