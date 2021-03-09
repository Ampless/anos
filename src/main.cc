// https://sourceforge.net/p/predef/wiki/Architectures/
#if defined(__amd64__) || \
    defined(__amd64) || \
    defined(__x86_64__) || \
    defined(__x86_64) || \
    defined(_M_X64) || \
    defined(_M_AMD64) || \
    defined(i386) || \
    defined(__i386) || \
    defined(__i386__) || \
    defined(__IA32__) || \
    defined(_M_I86) || \
    defined(_M_IX86) || \
    defined(__X86__) || \
    defined(_X86_) || \
    defined(__THW_INTEL__) || \
    defined(__I86__) || \
    defined(__INTEL__)
# define ANOS_X86_PC
#elif defined(__aarch64__)
# define ANOS_ARM64_RASPI
#else
//TODO: parse more archs
# error "Check the arch your're compiling for! It isn't AArch64 nor is it x86."
#endif

#include <kinclude.hh>

#ifdef ANOS_X86_PC
# error "Support for x86 is coming, but currently only RasPis are supported."
#endif

#ifdef ANOS_ARM64_RASPI
extern "C" void kmain(uint64_t /*dtb_ptr32*/,
                      uint64_t x1,
                      uint64_t /*x2*/,
                      uint64_t /*x3*/) {
        uint64_t start = clock();
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
                printf("%d bits\n", 32 + (
                       ass == 0 ? 0 :  ass == 1 ? 4 :
                       ass == 2 ? 8 :  ass == 3 ? 10 :
                       ass == 4 ? 12 : ass == 5 ? 16 : 20));
        else uart_puts("unknown\n");

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

        //the user wants some time to look at the picture
        usleep(1000000);

        for(uint32_t x = 0; x < gpu.width(); x++)
                for(uint32_t y = 0; y < gpu.height(); y++)
                        gpu.pixel(x, y) = rgba(x|rand(), y|rand(), (x^y)|rand(), 0xff);

        uint64_t end = clock();

        uart_puts("All of this took ");
        printf("%d milliseconds.\n", (end - start) / 1000 - 2000);

        printf("Allocated 100B each @ %lx & %lx\n", kalloc(100), kalloc(100));

        shutdown(false);
}
#endif
