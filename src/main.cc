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
extern "C" void kmain() {
        uint64_t start = clock();
        uart_init();

        // get the serial number with a mailbox call
        size_t idx = mbox_start(8);
        mbox_cmd(idx, MBOX_TAG_GETSERIAL, 2, 0, 0);
        mbox_end(idx);

        // send the message to the GPU and receive answer
        if (mbox_call(MBOX_CH_PROP))
                printf("My serial number is: %8x%8x\n", mbox[6], mbox[5]);
        else uart_puts("Unable to query serial!\n");

        srand();
        printf("Random number: %8x\n", rand());

        MemoryModel memmod;
        printf("physical address space: %d bits\n", memmod.pa_range());

        printf("4k granules %ssupported.\n", memmod.stage1_tgran4() ? "" : "not ");
        printf("16k granules %ssupported.\n", memmod.stage1_tgran16() ? "" : "not ");
        printf("64k granules %ssupported.\n", memmod.stage1_tgran64() ? "" : "not ");

        //TODO: check why this was happening
        //i really want to get rid of this, but it seems like i forgot
        //to wait for sth in gpu ctor, because it segfaults in qemu
        //doesnt seem to happen anymore
        //usleep(10000);

        GPU gpu;
        assert(gpu.valid());

        gpu.draw_picture([](uint32_t x, uint32_t y) {
                return rgba(x|rand(), y|rand(), (x^y)|rand(), 0xff);
        });

        gpu.showdemopicture();

        uint64_t end = clock();

        uart_puts("All of this took ");
        printf("%d milliseconds.\n", (end - start) / 1000);

        printf("Allocated 100B each @ %lx & %lx\n", kalloc(100), kalloc(100));

        shutdown(false);
}
#endif
