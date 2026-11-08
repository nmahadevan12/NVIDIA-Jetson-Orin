/* LOAD_ADDR is passed in by the Makefile (--defsym). It must be the
 * address the previous boot stage jumps to. */
ENTRY(_start)

SECTIONS
{
    . = LOAD_ADDR;

    .text   : { *(.text.boot) *(.text*) }
    .rodata : { *(.rodata*) }
    .data   : { *(.data*) }

    .bss (NOLOAD) :
    {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(16);
        __bss_end = .;
    }

    /* 16 KB stack, growing down from __stack_top */
    . = ALIGN(16);
    . += 0x4000;
    __stack_top = .;
}
