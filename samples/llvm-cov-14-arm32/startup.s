/**
 * arm926ej startup assembly code
 *
 * Lifted from "Hello world for bare metal ARM using QEMU"
 * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
 */
.global _Reset
_Reset:
 LDR sp, =stack_top
 BL c_entry
 B .
 