/* Touch subsystem linker script.
 *
 * Each function is linked at its fixed ROM address. The .text section
 * starts at the function's entry point. Data sections are placed after
 * .text for each object file.
 *
 * This linker script is used per-function to produce a flat binary
 * whose .text bytes can be compared against the original ROM extraction.
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY {
    /* Each function gets its own region at its ROM address.
     * The build system invokes the linker with -Ttext=<addr>. */
    RAM (rwx) : ORIGIN = 0x02000000, LENGTH = 0x86898
}

SECTIONS {
    .text : {
        *(.text*)
        *(.rodata*)
    } > RAM

    .data : {
        *(.data*)
    } > RAM

    .bss : {
        *(.bss*)
        *(COMMON)
    } > RAM
}
