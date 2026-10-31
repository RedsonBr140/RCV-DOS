#include <kernel/firmware.h>
#include <kernel/sbi.h>

const char *sbi_impl_name(unsigned long id) {
    switch (id) {
        case 0:
            return "Berkeley Boot Loader (BBL)";
        case 1:
            return "OpenSBI";
        case 2:
            return "Xvisor";
        case 3:
            return "KVM";
        case 4:
            return "RustSBI";
        case 5:
            return "Diosix";
        case 6:
            return "Coffer";
        case 7:
            return "Xen Project";
        case 8:
            return "PolarFire Hart Software Services";
        case 9:
            return "coreboot";
        case 10:
            return "oreboot";
        case 11:
            return "bhyve";
        default:
            return "Unknown implementation";
    }
}

struct sbi_version sbi_parse_spec_version(long version) {
    return (struct sbi_version) {
        .major = (version >> 24) & 0x7F,
        .minor = version & 0xFFFFFF
    };
}

unsigned int sbi_has_dbcn(void) {
    struct sbiret result = sbi_probe_extension(SBI_EXT_DBCN);
    return result.value;
}

_Noreturn void halt(void) {
    while (1)
        __asm__ volatile ("wfi");
}