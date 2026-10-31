#include <kernel/kprintf.h>

void kernel_main(void)
{
    kprintf("RCV-DOS booted!\n");

    kprintf("Signed: %d\n", -1234);
    kprintf("Unsigned: %u\n", 1234);
    kprintf("Hex: %x\n", 0xDEADBEEF);
    kprintf("Hex uppercase: %X\n", 0xDEADBEEF);

    kprintf("Character: %c\n", 'R');
    kprintf("String: %s\n", "RCV-DOS");
    kprintf("Percentage: 100%%\n");

    kprintf("INT_MIN: %d\n", (-2147483647 - 1));

    while (1)
        __asm__ volatile ("wfi");
}