#include <stdint.h>
#include <kernel/sbi.h>

#define SBI_EXT_DBCN 0x4442434E
#define SBI_DBCN_WRITE 0

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
