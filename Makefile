# Sonic Rush ARM9 Decompilation - Makefile
# Cross-compile with ARM GCC for NDS ARM9 (ARM946E-S, Thumb)

CC      = /home/tekkra/sonic-rush/tools/arm-gcc/bin/arm-none-eabi-gcc
OBJCOPY = /home/tekkra/sonic-rush/tools/arm-gcc/bin/arm-none-eabi-objcopy
OBJDUMP = /home/tekkra/sonic-rush/tools/arm-gcc/bin/arm-none-eabi-objdump
SIZE    = /home/tekkra/sonic-rush/tools/arm-gcc/bin/arm-none-eabi-size
LD      = /home/tekkra/sonic-rush/tools/arm-gcc/bin/arm-none-eabi-ld

# NDS ARM9 flags
ARCH    = -mthumb -mcpu=arm946e-s -march=armv5te -mlittle-endian
OPT     = -O2 -ffreestanding -nostdlib -fno-builtin -fno-common
WARN    = -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-sign-compare -Wno-incompatible-pointer-types -Wno-int-conversion -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
INCLUDE = -Iinclude -Isrc

CFLAGS  = $(ARCH) $(OPT) $(WARN) $(INCLUDE) -c

# Directories
SRCDIR  = src
OBJDIR  = build/obj
INCDIR  = include

# Find all C files
SRCS    = $(shell find $(SRCDIR) -name "*.c" | sort)
OBJS    = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# Subdirectories for build
SUBDIRS = $(sort $(dir $(OBJS)))

.PHONY: all clean info dirs

all: dirs $(OBJS)
	@echo "=== Build complete ==="
	@echo "Objects: $(words $(OBJS))"
	@$(SIZE) $(OBJS) 2>/dev/null | tail -1

dirs:
	@mkdir -p $(SUBDIRS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "  CC    $<"
	@$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJDIR)

# Compilation info
info:
	@echo "Source files: $(words $(SRCS))"
	@echo "Object files: $(words $(OBJS))"
	@echo "Compiler: $(CC)"
	@echo "Flags: $(ARCH) $(OPT)"

# Count functions per directory
stats:
	@echo "=== Functions per subsystem ==="
	@for d in $(SRCDIR)/*/; do \
		name=$$(basename $$d); \
		count=$$(find $$d -name "*.c" -exec grep -cE "^(void|s32|u32|s16|u16|s8|u8|int|char) [A-Za-z_]" {} + 2>/dev/null | awk -F: '{s+=$$2}END{print s}'); \
		fcount=$$(find $$d -name "*.c" | wc -l); \
		printf "  %-12s %4d files  %5d functions\n" "$$name" "$$fcount" "$$count"; \
	done
	@echo "---"
	@total_files=$$(find $(SRCDIR) -name "*.c" | wc -l); \
	total_funcs=$$(find $(SRCDIR) -name "*.c" -exec grep -cE "^(void|s32|u32|s16|u16|s8|u8|int|char) [A-Za-z_]" {} + 2>/dev/null | awk -F: '{s+=$$2}END{print s}'); \
	printf "  %-12s %4d files  %5d functions\n" "TOTAL" "$$total_files" "$$total_funcs"

# Try compile (report errors without stopping)
try: dirs
	@echo "=== Attempting compilation of all files ==="
	@fail=0; pass=0; total=0; \
	for src in $(SRCS); do \
		total=$$((total + 1)); \
		obj=$$(echo $$src | sed 's|$(SRCDIR)/|$(OBJDIR)/|; s|\.c$|.o|'); \
		if $(CC) $(CFLAGS) $$src -o $$obj 2>/dev/null; then \
			pass=$$((pass + 1)); \
		else \
			fail=$$((fail + 1)); \
		fi; \
	done; \
	echo "=== Results: $$pass passed, $$fail failed, $$total total ==="

# Link all objects into a relocatable ELF (partial link to find undefined symbols)
LINKOBJS = $(shell find $(OBJDIR) -name "*.o" | sort)
partial_link: all
	@echo "=== Partial linking to find undefined symbols ==="
	@$(LD) -r --allow-multiple-definition $(LINKOBJS) -o build/partial.o
	@echo "Undefined: $$(nm build/partial.o | grep ' U ' | wc -l)"
	@$(SIZE) build/partial.o

# Final binary output
final: partial_link
	@echo "=== Creating final binary ==="
	@$(OBJCOPY) -O binary build/partial.o build/sonic_rush_arm9.bin
	@echo "Binary size: $$(stat -c%s build/sonic_rush_arm9.bin) bytes"
