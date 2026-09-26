/* Sonic Rush ARM9 Linker Script */
/* Memory layout based on NDS ROM analysis */

MEMORY {
    /* NDS ARM9 main RAM */
    RAM (rwx) : ORIGIN = 0x02000000, LENGTH = 0x400000

    /* Overlay RAM */
    OVLY (rwx) : ORIGIN = 0x02100000, LENGTH = 0x100000
}

ENTRY(_start)

SECTIONS {
    /* Main code section */
    .text : {
        build/partial.o(.text)
        *(.text)
        *(.text.*)
    } > RAM

    /* Read-only data */
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > RAM

    /* Initialized data */
    .data : {
        *(.data)
        *(.data.*)
    } > RAM

    /* BSS (zero-initialized) */
    .bss : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end = .;
    } > RAM

    /* Stack */
    __stack_top = ORIGIN(RAM) + LENGTH(RAM);
}
