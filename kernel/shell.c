#include <kernel/shell.h>
#include <kernel/kprintf.h>
#include <kernel/sbi.h>
#include <stdint.h>
#include <kernel/firmware.h>

#define PS1 "RCV-DOS> "

// TODO: Should transfer those to an internal libK, which will be a minimal libC for the kernel itself.
size_t strlen(const char *str) {
    size_t count = 0;
    while(str[count] != '\0') {
        count++;
    }

    return count;
}

int strcmp(const char *str1, const char *str2) {
    int index = 0;

    while (str1[index] != '\0' || str2[index] != '\0') {
        if (str1[index] != str2[index]) {
            return -1;
        }

        index++;
    }

    return 0;
}

void shell_parse(char *line_buffer) {
    if (strcmp(line_buffer, "help") == 0) {
        kprintf("RCV-DOS shell v0.1 -- Hardcoded edition\n\n \
        help    -- gives you a summary of the available commands.\n \
        clear   -- clears the screen. \n \
        halt    -- halts the entire kernel.\n \
        version -- echoes the current OS version to the screen.\n");
    } else if (strcmp(line_buffer, "clear") == 0) {
        kprintf("\033[2J\033[H");
    } else if (strcmp(line_buffer, "version") == 0) {
        kprintf("RCV-DOS %s\n", RCV_DOS_VERSION);
    } else if (strcmp(line_buffer, "halt") == 0) {
        halt();
    } else {
        kprintf("Unknown command. Can't you fucking use the help command???\n");

    }
}

void shell_start(void) {
    kprintf("Starting the shell...\n");
    kprintf(PS1);

    static char input_buffer[128];
    // line_buffer will be passed to the parser as a valid C string, ending with a null terminator.
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
                        if (line_buffer_length > 0) {
                            line_buffer[line_buffer_length] = '\0';

                            kprintf("\n");
                            shell_parse(line_buffer);
                        }
                        // When the parse function returns, we re-print the shell (fake) PS1
                        kprintf("\n"); // NOTE: Alledgely, the command is supposed to end with a \n if it messes with the stdio either way, so we shouldn't need this.
                        kprintf(PS1);
                        // Then we empty the buffer.
                        for (int k=0;k<sizeof(line_buffer);k++) {
                            line_buffer[k] = 0;
                        }
                        line_buffer_length = 0;
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