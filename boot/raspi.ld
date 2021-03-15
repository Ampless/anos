/* IDK anything about ld, big thanks to OSDev Wiki! */
/* OK I think I have understood something, but clearly I'm clueless */

ENTRY(_start)
 
SECTIONS
{
    . = 0x80000;
    __start = .;
    .text : { KEEP(*(.text.boot)) *(.text) }
    . = ALIGN(4096);
 
    .rodata : { *(.rodata) }
    . = ALIGN(4096);
 
    .data : { *(.data) }
    . = ALIGN(4096);
 
    __bss_start = .;
    .bss : { bss = .; *(.bss) }
    . = ALIGN(4096);
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
    __end = .;
}
