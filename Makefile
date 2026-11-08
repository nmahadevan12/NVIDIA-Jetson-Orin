# macOS: brew install --cask gcc-aarch64-embedded   (provides aarch64-none-elf-*)
# Usage: make (defaults to 0x80000000) or make LOAD_ADDR=0x........
CROSS   ?= aarch64-none-elf-
CC      := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy

# Default to Jetson Orin Nano DRAM base address if not specified
LOAD_ADDR ?= 0x80000000

CFLAGS  := -ffreestanding -nostdlib -mgeneral-regs-only -mstrict-align \
           -O2 -Wall -Wextra
LDFLAGS := -nostdlib -T linker.ld -Wl,--defsym=LOAD_ADDR=$(LOAD_ADDR)

all: build/kernel.bin

build/start.o: src/start.S
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/main.o: src/main.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.elf: build/start.o build/main.o linker.ld
	$(CC) $(LDFLAGS) build/start.o build/main.o -o $@

build/kernel.bin: build/kernel.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf build