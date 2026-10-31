#ifndef KERNEL_SBI_H
#define KERNEL_SBI_H

#include <stddef.h>

struct sbiret
{
    long error;
    long value;
};

struct sbiret sbi_debug_console_write(
    size_t length,
    const void *buffer
);

#endif
