#pragma once
#include <stddef.h>

#define SBI_EXT_DBCN 0x4442434E

struct sbiret {
    long error;
    union {
        long value;
        unsigned long uvalue;
    };
};

struct sbiret sbi_get_spec_version(void);
struct sbiret sbi_get_impl_id(void);
struct sbiret sbi_probe_extension(long extension_id);

struct sbiret sbi_debug_console_write(
    size_t length,
    const void *buffer
);

struct sbiret sbi_debug_console_read(unsigned long num_bytes,
                                      unsigned long base_addr_lo,
                                      unsigned long base_addr_hi);