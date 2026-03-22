/**
 * C entry point
 */
volatile unsigned int *const UART0DR = (unsigned int *)0x101f1000;

typedef void (*fn_ptr)();

// Forward declaration for LLVM COV
extern void llvm_gcov_init(fn_ptr wfn, fn_ptr rfn);
extern void llvmcov_writeout(void);

// These symbols are defined in the linker script
extern fn_ptr __init_array_begin[];
extern fn_ptr __init_array_end[];

extern int main();

/**
 * Lifted from "Hello world for bare metal ARM using QEMU"
 * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
 */
void print_uart0(const char *s)
{
    while (*s != '\0')
    {                                  /* Loop until end of string */
        *UART0DR = (unsigned int)(*s); /* Transmit char */
        s++;                           /* Next char */
    }
}

/**
 * Adapted from "Hello world for bare metal ARM using QEMU"
 * https://balau82.wordpress.com/2010/02/28/hello-world-for-bare-metal-arm-using-qemu/
 */
static void call_init_functions(void)
{
    for (fn_ptr *fn = __init_array_begin; fn < __init_array_end; ++fn)
    {
        // Call the init
        (*fn)();
    }
}

void c_entry()
{
    // Call the functions in "init_array" of all linked object files
    call_init_functions();

    // Run the main application
    main();

    // Write LLVM COV info somehwere...
    llvmcov_writeout();
}