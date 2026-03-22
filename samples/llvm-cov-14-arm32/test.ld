/**
 * Adapted from "Hello world for bare metal ARM using QEMU"
 * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
 */
ENTRY(_Reset)
SECTIONS
{
 . = 0x10000;
 .startup . : { startup.o(.text) }
 .text : { *(.text) }
 .data : { *(.data) }
 .bss : { *(.bss COMMON) }
 . = ALIGN(8);

/**
 * Collect .init_array.* sections from object files into the binary while making 
 * the init functions accessible as external symbols in C code.
 */
 __init_array_begin = .;
 .init_array : { *(.init_array.*) }
 __init_array_end = .;

 . = . + 0x1000; /* 4kB of stack memory */
 stack_top = .;
}