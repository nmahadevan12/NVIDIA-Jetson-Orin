/* AArch64 entry point. Runs first, before any C code.
 * Assumes only the boot core arrives here (firmware keeps the other
 * cores parked until it starts them), so there is no core-ID check. */

.section .text.boot
.global _start

_start:
    /* Stack pointer: top of the stack area reserved in linker.ld */
    ldr     x0, =__stack_top
    mov     sp, x0

    /* Zero .bss, 8 bytes at a time (linker.ld keeps it 16-byte aligned) */
    ldr     x0, =__bss_start
    ldr     x1, =__bss_end
1:  cmp     x0, x1
    b.hs    2f
    str     xzr, [x0], #8
    b       1b

2:  bl      main

    /* If main ever returns, park the core */
3:  wfe
    b       3b