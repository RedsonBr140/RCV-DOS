#include <kernel/shell.h>
#include <kernel/kprintf.h>
#include <kernel/sbi.h>
#include <stdint.h>

void shell_start(void) {
    kprintf("Starting the shell...\n");
    kprintf("RCV-DOS> ");

    static char input_buffer[128];
    while(1) {
        struct sbiret result = sbi_debug_console_read(
            sizeof(input_buffer),
            (unsigned long)(uintptr_t)input_buffer,
            0
        );
    }
};