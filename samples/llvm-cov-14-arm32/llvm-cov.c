/**
 * llvm-cov utilities
 * 
 * Most code adapted from llvm-project/compiler-rt/lib/profile/GCDAProfiling.c (tag llvmorg-14.0.0)
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*fn_ptr)();

// Preallocated buffers to register llvm-cov callback functions
#define MAX_FUNCTIONS 256
static fn_ptr g_wfn[MAX_FUNCTIONS];
static fn_ptr g_rfn[MAX_FUNCTIONS];

// Coverage data dump buffer
#define COV_DATA_DUMP_BUFFER_SIZE 1048576
static char g_cov_data_dump_buffer[COV_DATA_DUMP_BUFFER_SIZE];
static size_t g_cov_data_next_index = 0;

// Index into g_cov_data_dump_buffer of the last "file size" uint32 from the last call to llvm_gcda_start_file
static size_t g_last_file_size_index = 0;

// gcov version determined from the last call to llvm_gcda_start_file
static uint32_t gcov_version;

// Constants taken from llvm-project/compiler-rt/lib/profile/GCDAProfiling.c (tag llvmorg-14.0.0)
enum
{
    GCOV_DATA_MAGIC = 0x67636461, // "gcda"
    GCOV_TAG_FUNCTION = 0x01000000,
    GCOV_TAG_COUNTER_ARCS = 0x01a10000,
    // GCOV_TAG_OBJECT_SUMMARY superseded GCOV_TAG_PROGRAM_SUMMARY in GCC 9.
    GCOV_TAG_OBJECT_SUMMARY = 0xa1000000,
    GCOV_TAG_PROGRAM_SUMMARY = 0xa3000000,
};

///////////////////////////////////
// Debug print
extern void print_uart0(const char *s);

///////////////////////////////////
// Forward declarations
static void __llvmcov_add(fn_ptr *array, fn_ptr fn);
static void __llvmcov_write_all(void);
static void __llvmcov_reset_all(void);
static void __llvmcov_clear(void);

static void __llvmcov_write_string(const char *str);
static void __llvmcov_write_u32(uint32_t value);
static void __llvmcov_write_u64(uint64_t value);
static void __llvmcov_write_u32_at_index(uint32_t value, uint32_t index);
static void __llvmcov_write_u64_at_index(uint64_t value, uint32_t index);

static size_t __strlen(const char *str);
static void __insert_cov_data_dump(void *str, size_t len);
static void __insert_cov_data_dump_at_index(void *buf, size_t len, size_t index);
static char __get_hexdigit(uint8_t value);
static void __printhex(uint8_t value, char *buf);
static void __hexdump(const char *buf, size_t len);

///////////////////////////////////
void llvm_gcov_init(fn_ptr wfn, fn_ptr rfn)
{
    print_uart0("[llvm_gcov_init]\n");
    if (wfn)
    {
        print_uart0("[llvm_gcov_init] wfn not null\n");
        __llvmcov_add(g_wfn, wfn);
    }
    if (rfn)
    {
        print_uart0("[llvm_gcov_init] rfn not null\n");
        __llvmcov_add(g_rfn, rfn);
    }
}

void llvm_gcda_start_file(const char *orig_filename, uint32_t version, uint32_t checksum)
{
    print_uart0("[llvm_gcda_start_file]\n");
    __llvmcov_write_string(orig_filename);

    // Placeholder for file size
    g_last_file_size_index = g_cov_data_next_index;
    __llvmcov_write_u32(0);

    /* gcda file, version, stamp checksum. */
    {
        uint8_t c3 = version >> 24;
        uint8_t c2 = (version >> 16) & 255;
        uint8_t c1 = (version >> 8) & 255;
        gcov_version = c3 >= 'A' ? (c3 - 'A') * 100 + (c2 - '0') * 10 + c1 - '0'
                                 : (c3 - '0') * 10 + c1 - '0';
    }
    __llvmcov_write_u32(GCOV_DATA_MAGIC);
    __llvmcov_write_u32(version);
    __llvmcov_write_u32(checksum);
}

void llvm_gcda_end_file()
{
    print_uart0("[llvm_gcda_end_file]\n");

    /* Write out EOF record. */
    __llvmcov_write_u64(0);

    // Write the file size at the last place holder
    size_t filesize = g_cov_data_next_index - g_last_file_size_index - 4;
    __llvmcov_write_u32_at_index(filesize, g_last_file_size_index);
}

void llvm_gcda_emit_function(uint32_t ident, uint32_t func_checksum, uint32_t cfg_checksum)
{
    print_uart0("[llvm_gcda_emit_function]\n");
    uint32_t len = 2;
    int use_extra_checksum = gcov_version >= 47;

    if (use_extra_checksum)
    {
        len++;
    }

    /* function tag */
    __llvmcov_write_u32(GCOV_TAG_FUNCTION);
    __llvmcov_write_u32(len);
    __llvmcov_write_u32(ident);
    __llvmcov_write_u32(func_checksum);
    if (use_extra_checksum)
    {
        __llvmcov_write_u32(cfg_checksum);
    }
}

void llvm_gcda_emit_arcs(uint32_t num_counters, uint64_t *counters)
{
    print_uart0("[llvm_gcda_emit_arcs]\n");

    // Assume new file... no counters to merge

    /* Counter #1 (arcs) tag */
    __llvmcov_write_u32(GCOV_TAG_COUNTER_ARCS);
    __llvmcov_write_u32(num_counters * 2);
    for (int i = 0; i < num_counters; ++i)
    {
        __llvmcov_write_u64(counters[i]);
    }
}

void llvm_gcda_summary_info()
{
    print_uart0("[llvm_gcda_summary_info]\n");
    uint32_t runs = 1;

    // Assume new file... no counters to merge

    if (gcov_version >= 90)
    {
        __llvmcov_write_u32(GCOV_TAG_OBJECT_SUMMARY);
        __llvmcov_write_u32(2);
        __llvmcov_write_u32(runs);
        __llvmcov_write_u32(0); // sum_max
    }
    else
    {
        // Before gcov 4.8 (r190952), GCOV_TAG_SUMMARY_LENGTH was 9. r190952 set
        // GCOV_TAG_SUMMARY_LENGTH to 22. We simply use the smallest length which
        // can make gcov read "Runs:".
        __llvmcov_write_u32(GCOV_TAG_PROGRAM_SUMMARY);
        __llvmcov_write_u32(3);
        __llvmcov_write_u32(0);
        __llvmcov_write_u32(0);
        __llvmcov_write_u32(runs);
    }
}

void __aeabi_memclr(void *dest, size_t n)
{
    print_uart0("[__aeabi_memclr]\n");
    char *c = (void *)dest;
    for (size_t i = 0; i < n; i++)
    {
        *c = 0;
    }
}

void llvmcov_writeout(void)
{
    print_uart0("[llvmcov_writeout]\n");

    // Call all the registered "write" callback functionss
    __llvmcov_write_all();

    // Hex dump coverage data on console
    __hexdump(g_cov_data_dump_buffer, g_cov_data_next_index);

    // Call all the registered "reset" callback functionss
    __llvmcov_reset_all();
}

///////////////////////////////////

static void __llvmcov_add(fn_ptr *array, fn_ptr fn)
{
    bool added = false;
    print_uart0("[__llvmcov_add]\n");
    for (int i = 0; i < MAX_FUNCTIONS; i++)
    {
        if (array[i] == NULL)
        {
            array[i] = fn;
            added = true;
            break;
        }
    }

    if (!added)
    {
        // If we get here, that means array is full
        print_uart0("[__llvmcov_add] ERROR: array full\n");
    }
}

static void __llvmcov_write_all(void)
{
    print_uart0("[__llvmcov_write_all]\n");
    for (int i = 0; i < MAX_FUNCTIONS; i++)
    {
        fn_ptr fn = g_wfn[i];
        if (fn == NULL)
        {
            // Reached the end of the entries
            break;
        }
        fn();
    }
}

static void __llvmcov_reset_all(void)
{
    print_uart0("[__llvmcov_reset_all]\n");
    for (int i = 0; i < MAX_FUNCTIONS; i++)
    {
        fn_ptr fn = g_rfn[i];
        if (fn == NULL)
        {
            // Reached the end of the entries
            break;
        }
        fn();
    }
}

static void __llvmcov_clear(void)
{
    print_uart0("[__llvmcov_clear]\n");
    __aeabi_memclr(g_wfn, sizeof(g_wfn));
    __aeabi_memclr(g_rfn, sizeof(g_rfn));
}

static size_t __strlen(const char *str)
{
    size_t i = 0;
    for (; *(str + i) != 0; i++)
    {
        // ...
    }

    return i;
}

static void __insert_cov_data_dump_at_index(void *buf, size_t len, size_t index)
{
    if (index + len >= COV_DATA_DUMP_BUFFER_SIZE)
    {
        print_uart0("[__insert_cov_data_dump_at_index] coverage data buffer too small\n");
        return;
    }

    const char *src = buf;
    char *dst = g_cov_data_dump_buffer + index;
    for (size_t i = 0; i < len; i++)
    {
        *dst = *src;
        ++dst;
        ++src;
    }
}

static void __insert_cov_data_dump(void *buf, size_t len)
{
    __insert_cov_data_dump_at_index(buf, len, g_cov_data_next_index);
    g_cov_data_next_index += len;
}

/**
 * Write the null-terminated string, including the null byte, to coverage data dump buffer
 */
static void __llvmcov_write_string(const char *str)
{
    size_t len = __strlen(str);
    __insert_cov_data_dump((void *)str, len + 1);
}

static void __llvmcov_write_u32(uint32_t value)
{
    __insert_cov_data_dump(&value, 4);
}

static void __llvmcov_write_u64(uint64_t value)
{
    uint32_t lo = value & 0xffffffff;
    uint32_t hi = (value >> 32) & 0xffffffff;
    __insert_cov_data_dump(&lo, 4);
    __insert_cov_data_dump(&hi, 4);
}

static void __llvmcov_write_u32_at_index(uint32_t value, uint32_t index)
{
    __insert_cov_data_dump_at_index(&value, 4, index);
}

static void __llvmcov_write_u64_at_index(uint64_t value, uint32_t index)
{
    uint32_t lo = value & 0xffffffff;
    uint32_t hi = (value >> 32) & 0xffffffff;
    __insert_cov_data_dump_at_index(&lo, 4, index);
    __insert_cov_data_dump_at_index(&hi, 4, index + 4);
}

static char __get_hexdigit(uint8_t value)
{
    if (value < 10)
    {
        return '0' + value;
    }

    if (value < 16)
    {
        return 'A' + (value - 10);
    }

    return 'x';
}

static void __printhex(uint8_t value, char *buf)
{
    buf[0] = __get_hexdigit((value >> 4) & 0xf);
    buf[1] = __get_hexdigit(value & 0xf);
}

static void __hexdump(const char *buf, size_t len)
{
    const int COLS = 16;
    size_t rows = len / COLS;
    size_t remainder = len % COLS;
    const char *ptr = buf;

    // Print the complete rows
    for (int r = 0; r < rows; r++)
    {
        // Print hex numbers
        for (int c = 0; c < COLS; c++)
        {
            char ch = buf[r * COLS + c];
            char buf[4] = "   ";
            __printhex(ch, buf);
            print_uart0(buf);
        }

        print_uart0(" | ");

        // Print ASCII
        for (int c = 0; c < COLS; c++)
        {
            char ch = buf[r * COLS + c];
            char buf[2] = " ";
            buf[0] = (ch < 32 || ch > 127) ? '.' : ch;
            print_uart0(buf);
        }

        print_uart0("\n");
    }

    // Print the last partial rows
    if (remainder > 0)
    {
        // Print hex numbers
        int c = 0;
        for (; c < remainder; c++)
        {
            char ch = buf[rows * COLS + c];
            char buf[4] = "   ";
            __printhex(ch, buf);
            print_uart0(buf);
        }

        for (; c < COLS; c++)
        {
            char buf[4] = "   ";
            print_uart0(buf);
        }

        print_uart0(" | ");

        // Print ASCII
        for (int c = 0; c < remainder; c++)
        {
            char ch = buf[rows * COLS + c];
            char buf[2] = " ";
            buf[0] = (ch < 32 || ch > 127) ? '.' : ch;
            print_uart0(buf);
        }

        print_uart0("\n");
    }
}
