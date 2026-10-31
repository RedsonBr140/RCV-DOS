#include <kernel/console.h>
#include <kernel/sbi.h>

void console_write(const char *buffer, size_t length)
{
    while (length > 0)
    {
        struct sbiret result =
            sbi_debug_console_write(length, buffer);

        if (result.error != 0 || result.value <= 0)
            return;

        size_t written = (size_t)result.value;

        buffer += written;
        length -= written;
    }
}

