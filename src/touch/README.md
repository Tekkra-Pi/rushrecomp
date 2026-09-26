# Touch Subsystem Decompilation

Source-level decompilation of the Sonic Rush NDS touch input pipeline.

## Functions

| Address | Size | Name | Status |
|---------|------|------|--------|
| `0x02009900` | 440B | Sampler (parse shared samples) | skeleton |
| `0x0203363c` | 140B | Producer (init dispatcher node) | skeleton |
| `0x02033a68` | 188B | MainUpdater (bulk memcpy) | skeleton |
| `0x02033b98` | 436B | Dispatcher (validate + copy) | skeleton |
| `0x02033d58` | 80B | ResetEnable (walk nodes) | skeleton |
| `0x02033ddc` | 96B | Init (setup pipeline) | skeleton |

## Building

Requires `arm-none-eabi-gcc`:

```sh
cd src/touch
make
```

Each function is compiled separately, linked at its ROM address, and
the `.text` section bytes are compared against the original extraction
in `disassembly/<addr>.bin`.

## File Structure

```
src/touch/
├── Makefile          # byte-matching build rules
├── touch.ld          # linker script
├── touch_sampler.c   # FUN_02009900
├── touch_producer.c  # FUN_0203363c
├── touch_main.c      # FUN_02033a68
├── touch_dispatch.c  # FUN_02033b98
├── touch_reset.c     # FUN_02033d58
└── touch_init.c      # FUN_02033ddc

include/
├── nds_types.h       # NDS type definitions
└── touch_state.h     # Touch structs and flags
```

## Workflow

1. Ghidra headless decompilation produces C pseudocode
2. Manual translation to matching C source (this tree)
3. Compile with `arm-none-eabi-gcc -O0` at the function's address
4. Extract `.text` and compare byte-for-byte to `disassembly/<addr>.bin`
5. Iterate on compiler flags / source until all functions match

## Notes

- `-O0` is used initially; matching may require `-O1` or specific flags
- Each function is a separate compilation unit (no inlining across functions)
- External functions are declared as stubs; resolved at final link time
- The `ARM9` attribute forces ARM mode (not Thumb)
