#include <stdarg.h>
#include <kernel/kprintf.h>
#include <kernel/console.h>

static void print_unsigned (unsigned long value, unsigned int base, int uppercase) {
    const char *digits_lower = "0123456789abcdef";
    const char *digits_upper = "0123456789ABCDEF";

    const char * digits = uppercase ? digits_upper : digits_lower;

    char buffer[sizeof(unsigned long) * 8];
    size_t i = 0;

    do {
        buffer[i++] = digits[value % base];
        value /= base;
    } while (value != 0);

    while (i > 0) {
        i--;
        console_write(&buffer[i], 1);
    }
}

void kprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format != '%') {
            console_write(format, 1);
            format++;
            continue;
        }

        format++;

        if (*format == '\0') {
            console_write("%", 1);
            break;
        }

        switch (*format) {
            case '%':
                console_write("%", 1);
                break;
            case 'c': {
                char c = (char)va_arg(args, int);
                console_write(&c, 1);
                break;
            }

            case 's': {
                const char *str = va_arg(args, const char *);

                if (str == NULL) {
                    str = "(null)";
                }

                size_t len = 0;

                while(str[len]) {
                    len++;
                }

                console_write(str, len);
                break;
            }
            case 'd': {
                int value = va_arg(args, int);

                if (value < 0) {
                    console_write("-", 1);

                    unsigned int magnitude = 0u - (unsigned int)value;

                    print_unsigned(magnitude, 10, 0);
                } else {
                    print_unsigned((unsigned int)value, 10, 0);
                }
                break;
            }
            case 'u': {
                unsigned int value = va_arg(args, unsigned int);
                print_unsigned(value, 10, 0);
                break;
            }
            case 'x': {
                unsigned int value = va_arg(args, unsigned int);
                print_unsigned(value, 16, 0);
                break;
            }
            case 'X': {
                unsigned int value = va_arg(args, unsigned int);
                print_unsigned(value, 16, 1);
                break;
            }

            default:
                console_write("%", 1);
                console_write(format, 1);
                break;
        }

        format++;
    }
    va_end(args);
}
