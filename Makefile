CROSS = /opt/riscv/bin/riscv64-unknown-elf-

CC = $(CROSS)gcc
QEMU = qemu-system-riscv64

BUILD = build
KERNEL = $(BUILD)/kernel.elf

ARCH = rv64imac
ABI = lp64

CFLAGS = -march=$(ARCH) -mabi=$(ABI) \
         -mcmodel=medany \
         -ffreestanding -fno-builtin \
         -fno-stack-protector \
         -O2 -Wall -Wextra \
         -Iinclude

ASFLAGS = -march=$(ARCH) -mabi=$(ABI)

LDFLAGS = -T linker.ld -nostdlib -static \
          -Wl,--build-id=none \
          -Wl,-Map=$(BUILD)/kernel.map

C_SOURCES = \
    kernel/kernel_main.c \
    kernel/kprintf.c \
    drivers/console.c \
    arch/riscv/sbi.c

ASM_SOURCES = \
    arch/riscv/boot.S

OBJECTS = $(patsubst %.c,$(BUILD)/%.o,$(C_SOURCES)) \
          $(patsubst %.S,$(BUILD)/%.o,$(ASM_SOURCES))

.PHONY: all run debug clean

all: $(KERNEL)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

$(KERNEL): $(OBJECTS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

run: $(KERNEL)
	$(QEMU) -machine virt -cpu rv64 -m 128M \
		-smp 4 -nographic -bios default -kernel $(KERNEL)

debug: $(KERNEL)
	$(QEMU) -machine virt -cpu rv64 -m 128M \
		-smp 4 -nographic -bios default -kernel $(KERNEL) \
		-S -s

clean:
	rm -rf $(BUILD)



