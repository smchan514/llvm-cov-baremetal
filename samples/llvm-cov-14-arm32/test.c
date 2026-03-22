/**
 * Sample C application
 */
extern void print_uart0(const char *s);

static void func1(void)
{
    print_uart0("func1\n");
}

static void func2(void)
{
    print_uart0("func2\n");
}

int main()
{
    print_uart0("Hello world!\n");
    for (int i = 0; i < 10; i++)
    {
        if ((i % 2) == 0)
        {
            func1();
        }
        else
        {
            func2();
        }
    }
}