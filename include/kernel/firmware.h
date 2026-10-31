#pragma once

struct sbi_version {
    unsigned int major;
    unsigned int minor;
};

const char *sbi_impl_name(unsigned long id);
struct sbi_version sbi_parse_spec_version(long value);

unsigned int sbi_has_dbcn(void);

void halt(void);