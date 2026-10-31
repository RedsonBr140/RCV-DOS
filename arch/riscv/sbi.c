#include <stdint.h>
#include <kernel/sbi.h>

#define SBI_EXT_BASE              0x10
#define SBI_BASE_GET_SPEC_VERSION 0x00
#define SBI_BASE_GET_IMPL_ID      0x01
#define SBI_BASE_PROBE_EXTENSION  0x03

#define SBI_DBCN_WRITE 0x00
#define SBI_DBCN_READ  0x01

static struct sbiret sbi_call(
    unsigned long eid,
    unsigned long fid,
    unsigned long arg0,
    unsigned long arg1,
    unsigned long arg2)
{
    register unsigned long a0 asm("a0") = arg0;
    register unsigned long a1 asm("a1") = arg1;
    register unsigned long a2 asm("a2") = arg2;
    register unsigned long a6 asm("a6") = fid;
    register unsigned long a7 asm("a7") = eid;

    asm volatile (
        "ecall"
        : "+r"(a0), "+r"(a1)
        : "r"(a2), "r"(a6), "r"(a7)
        : "memory"
    );

    return (struct sbiret) {
        .error = (long)a0,
        .value = (long)a1
    };
}
struct sbiret sbi_get_impl_id(void) {
    return sbi_call(
        SBI_EXT_BASE,
        SBI_BASE_GET_IMPL_ID,
        0,
        0,
        0
    );
}
struct sbiret sbi_get_spec_version(void) {
    return sbi_call(
        SBI_EXT_BASE,
        SBI_BASE_GET_SPEC_VERSION,
        0,
        0,
        0
    );
}

struct sbiret sbi_probe_extension(long extension_id) {
    return sbi_call(
        SBI_EXT_BASE,
        SBI_BASE_PROBE_EXTENSION,
        extension_id,
        0,
        0
    );
}

struct sbiret sbi_debug_console_write(
    size_t length,
    const void *buffer)
{
    return sbi_call(
        SBI_EXT_DBCN,
        SBI_DBCN_WRITE,
        length,
        (unsigned long)(uintptr_t)buffer,
        0
    );
}

struct sbiret sbi_debug_console_read(unsigned long num_bytes, unsigned long base_addr_lo, unsigned long base_addr_hi) {
    return sbi_call(
        SBI_EXT_DBCN,
        SBI_DBCN_READ,
        num_bytes,
        base_addr_lo,
        base_addr_hi
    );
}
