#include <kernel/shell.h>
#include <kernel/kprintf.h>
#include <kernel/sbi.h>
#include <stdint.h>

#define PS1 "RCV-DOS> "
void shell_start(void) {
    kprintf("Starting the shell...\n");
    kprintf(PS1);

    static char input_buffer[128];
    /* line_buffer isn't supposed to be a valid C String. It's a buffer afterall.
     * We'll consider it as full if it ends with a New Line Character ('\n'). That means
     * we're ready to parse the command.
     */
    static char line_buffer[128];
    static size_t line_buffer_length;
    while(1) {
        struct sbiret result = sbi_debug_console_read(
            sizeof(input_buffer),
            (unsigned long)(uintptr_t)input_buffer,
            0
        );

        if (result.error == 0 && result.uvalue > 0) {
            for (int i=0;i < result.uvalue;i++) {
                switch(input_buffer[i]) {
                    case 0x7F:
                    case '\b':
                        if (line_buffer_length > 0) {
                            kprintf("\b \b");
                            line_buffer_length--;
                        }
                        break;
                    case '\r':
                        // TODO: Parse the command.
                        // When the parse function returns, we re-print the shell (fake) PS1
                        kprintf("\n"); // NOTE: Alledgely, the command is supposed to end with a \n if it messes with the stdio either way, so we shouldn't need this.
                        kprintf(PS1);
                        // Then "empty" the buffer.
                        line_buffer_length = 0;
                        break;
                    case 'L':
                        kprintf("Pressing 'L' is a helper condition. Printing Line buffer:\n");
                        for (int k=0;k<line_buffer_length;k++) {
                            kprintf("%c(0x%x) ", line_buffer[k], line_buffer[k]);
                        }
                        break;
                    default:
                        if(line_buffer_length < sizeof(line_buffer) - 1) {
                            line_buffer[line_buffer_length] = input_buffer[i];
                            line_buffer_length++;
                            sbi_debug_console_write(1, &input_buffer[i]);
                        }

                        break;
                }
            }
        }
    }
};