// "I don't understand any of this, but it's from OSDev Wiki and I
// trust it." – chrissx, after they copy-pasted non-working code
// "I don't understand any of this, but it's from jsandler18 and I
// trust it." – chrissx, after they copy-pasted working code
// "This is horrible, ARM 32 <-> 64 bit differences are a fucking
// mess, I should go back to using AArch64 code" – chrissx, after they
// discovered, how bad jsandler18's code is for RPI3+.
// "ICH HAB N OBJECT FILE!" – chrissx, after fixing OSDev Wiki's code

.section ".text.boot"

.globl _start

.org 0x80000

_start:
        // grow stack from 0x80000 downwards
        ldr x5, =_start
        mov sp, x5

        ldr x5, =__bss_start
        ldr w6, =__bss_size
3:      cbz w6, 4f
        str xzr, [x5], #8
        sub w6, w6, #1
        cbnz w6, 3b

        // on core 0 run kernel_main, on the others do nothing
4:      bl kmain
1:      wfe
        b 1b

