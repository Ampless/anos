#include <cstddef>
#include <cstdint>

#define RASPI_TARGET 3

// this is an awful hack to stop the compiler from bloating the binary
namespace {
        volatile uint32_t &mem(const uint32_t loc) {
                return *reinterpret_cast<uint32_t *>(loc);
        }
        constexpr uint32_t bit(uint32_t i) {
                return 1 << i;
        }
        void delay(int32_t count) {
	        asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" : "=r"(count): [count]"0"(count) : "cc");
        }
}

enum {
#if RASPI_TARGET == 3
        MMIO_BASE    = 0x3F000000,
#elif RASPI_TARGET == 4
        MMIO_BASE    = 0xFE000000,
#else
#error "No, you don't want to use non RasPi 3/4 ones."
#endif

        GPIO_BASE    = (MMIO_BASE + 0x200000),
        GPPUD        = (GPIO_BASE + 0x94),
        GPPUDCLK0    = (GPIO_BASE + 0x98),
        UART0_BASE   = (GPIO_BASE + 0x1000),
        UART0_DR     = (UART0_BASE + 0x00),
        UART0_RSRECR = (UART0_BASE + 0x04),
        UART0_FR     = (UART0_BASE + 0x18),
        UART0_ILPR   = (UART0_BASE + 0x20),
        UART0_IBRD   = (UART0_BASE + 0x24),
        UART0_FBRD   = (UART0_BASE + 0x28),
        UART0_LCRH   = (UART0_BASE + 0x2C),
        UART0_CR     = (UART0_BASE + 0x30),
        UART0_IFLS   = (UART0_BASE + 0x34),
        UART0_IMSC   = (UART0_BASE + 0x38),
        UART0_RIS    = (UART0_BASE + 0x3C),
        UART0_MIS    = (UART0_BASE + 0x40),
        UART0_ICR    = (UART0_BASE + 0x44),
        UART0_DMACR  = (UART0_BASE + 0x48),
        UART0_ITCR   = (UART0_BASE + 0x80),
        UART0_ITIP   = (UART0_BASE + 0x84),
        UART0_ITOP   = (UART0_BASE + 0x88),
        UART0_TDR    = (UART0_BASE + 0x8C),
        MBOX_BASE    = (MMIO_BASE + 0xB880),
        MBOX_READ    = (MBOX_BASE + 0x00),
        MBOX_STATUS  = (MBOX_BASE + 0x18),
        MBOX_WRITE   = (MBOX_BASE + 0x20)
};

volatile unsigned int  __attribute__((aligned(16))) mbox[9] = {
    9*4, 0, 0x38002, 12, 8, 2, 3000000, 0 ,0
};

void uart_init() {
        mem(UART0_CR) = 0;
        mem(GPPUD) = 0;
        delay(150);
        mem(GPPUDCLK0) = (1 << 14) | (1 << 15);
        delay(150);
        mem(GPPUDCLK0) = 0;
        mem(UART0_ICR) = 0x7FF;
        uint32_t r = (((uint64_t)(&mbox) & ~0xF) | 8);
        while(mem(MBOX_STATUS) & 0x80000000);
        mem(MBOX_WRITE) = r;
        while((mem(MBOX_STATUS) & 0x40000000) || mem(MBOX_READ) != r);
	mem(UART0_IBRD) = 1;
	mem(UART0_FBRD) = 40;
	mem(UART0_LCRH) = bit(4) | bit(5) | bit(6);
	mem(UART0_IMSC) = bit(1) | bit(4) | bit(5) | bit(6) |
	                  bit(7) | bit(8) | bit(9) | bit(10);
	mem(UART0_CR)   = bit(0) | bit(8) | bit(9);
}

void uart_putc(char c) {
	while(mem(UART0_FR) & bit(5));
	mem(UART0_DR) = c;
}

char uart_getc() {
    while(mem(UART0_FR) & bit(4));
    return mem(UART0_DR);
}

void uart_puts(const char *str) {
	for (; *str != '\0'; str++) {
                uart_putc(*str);
        }
}

extern "C" void kernel_main(uint64_t dtb_ptr32,
                            uint64_t x1,
                            uint64_t x2,
                            uint64_t x3) {
        uart_init();
        uart_puts("Hello, kernel World!\n");
        while(1) uart_putc(uart_getc());
}
