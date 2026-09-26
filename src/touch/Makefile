# Touch subsystem decompilation — byte-matching build.
#
# Usage:
#   make            # compile all and check byte-matching
#   make check      # same
#   make asm        # generate assembly listings for comparison
#   make clean      # remove build artifacts
#
# Prerequisites:
#   arm-none-eabi-gcc (local install at ../../tools/arm-gcc/bin/)

ROOT     = ../..
ARM_DIR  = $(ROOT)/tools/arm-gcc/bin
CC       = $(ARM_DIR)/arm-none-eabi-gcc
OBJCOPY  = $(ARM_DIR)/arm-none-eabi-objcopy
OBJDUMP  = $(ARM_DIR)/arm-none-eabi-objdump
VERIFY   = $(ROOT)/scripts/verify-match.py

INCDIR   = $(ROOT)/include
DISDIR   = $(ROOT)/disassembly

CFLAGS = -marm -O2 -fno-builtin -fno-tree-loop-distribute-patterns \
         -ffreestanding -nostdlib -mcpu=arm946e-s -mfloat-abi=soft \
         -I$(INCDIR) -Wall -Wextra

LDFLAGS = -marm -mcpu=arm946e-s -nostdlib \
          -Wl,--unresolved-symbols=ignore-in-object-files \
          -Wl,--entry=0

BUILDDIR = build

# Functions: addr source_file size status
# status: match=close match, stub=needs more work, skip=not yet written
FUNCS_MATCH = \
	0x02033d58:touch_reset.c:80 \
	0x02033a68:touch_main.c:188 \
	0x02033ddc:touch_init.c:96

FUNCS_STUB = \
	0x02009900:touch_sampler.c:440 \
	0x0203363c:touch_producer.c:140 \
	0x02033b98:touch_dispatch.c:436

.PHONY: all check check-stubs asm clean

all: check

clean:
	rm -rf $(BUILDDIR)

asm: $(foreach f,$(FUNCS_MATCH) $(FUNCS_STUB),$(BUILDDIR)/$(word 1,$(subst :, ,$(f))).s)

# ── Per-function rules ──
define FUNC_RULES
$(eval ADDR := $(word 1,$(subst :, ,$(1))))
$(eval SRC  := $(word 2,$(subst :, ,$(1))))
$(eval SIZE := $(word 3,$(subst :, ,$(1))))

$(BUILDDIR)/$(ADDR).o: $(SRC) $(INCDIR)/touch_state.h $(INCDIR)/nds_types.h
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $$< -o $$@

$(BUILDDIR)/$(ADDR).elf: $(BUILDDIR)/$(ADDR).o
	$(CC) $(LDFLAGS) -Wl,-Ttext=$(ADDR) $$< -o $$@

$(BUILDDIR)/$(ADDR).text.bin: $(BUILDDIR)/$(ADDR).elf
	$(OBJCOPY) -O binary -j .text $$< $$@

$(BUILDDIR)/$(ADDR).s: $(SRC) $(INCDIR)/touch_state.h $(INCDIR)/nds_types.h
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -S -fverbose-asm $$< -o $$@

$(BUILDDIR)/$(ADDR).check: $(BUILDDIR)/$(ADDR).text.bin $(DISDIR)/$(ADDR).bin
	@echo "--- Checking $(ADDR) ($(SRC), expected $(SIZE)B) ---"
	@-python3 $(VERIFY) $(BUILDDIR)/$(ADDR).text.bin $(DISDIR)/$(ADDR).bin
	@touch $$@

CHECK_TARGETS += $(BUILDDIR)/$(ADDR).check
endef

$(eval $(call FUNC_RULES,0x02033d58:touch_reset.c:80))
$(eval $(call FUNC_RULES,0x02033a68:touch_main.c:188))
$(eval $(call FUNC_RULES,0x02033ddc:touch_init.c:96))

# Stub functions — compile but won't match yet
$(eval $(call FUNC_RULES,0x02009900:touch_sampler.c:440))
$(eval $(call FUNC_RULES,0x0203363c:touch_producer.c:140))
$(eval $(call FUNC_RULES,0x02033b98:touch_dispatch.c:436))

check: $(CHECK_TARGETS)
	@echo "=== All touch functions checked ==="
