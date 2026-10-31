#include <kernel/kprintf.h>
#include <kernel/sbi.h>
#include <kernel/firmware.h>
#include <stdint.h>
#include <kernel/shell.h>

void kernel_main(void) {
    if(!sbi_has_dbcn()) {
        halt();
    }
    kprintf("\033[2J\033[H"); // Erase the terminal using ANSI escape
    kprintf("RCV-DOS booted!\n");

    const char *spec_name = sbi_impl_name(sbi_get_impl_id().value);
    struct sbi_version version = sbi_parse_spec_version(sbi_get_spec_version().value);

    kprintf("%s Version %d.%d\n", spec_name, version.major, version.minor);

    shell_start();

    /*
    

    while(1) {
        struct sbiret result = sbi_debug_console_read(
            sizeof(input_buffer),
            (unsigned long)(uintptr_t)input_buffer,
            0
        );

        if (result.value > 0) {
            sbi_debug_console_write(sizeof(input_buffer), input_buffer);
        }
    }
    */
}