# Touch Subsystem — Matching Notes

## Status

All 6 touch functions compile and link correctly. Byte-level matching
against the original ROM is not yet achieved — the original was compiled
with Metrowerks CodeWarrior for NDS, while we use GCC 13.3.

### Size comparison (all functions complete)

| Function | Address | Expected | Compiled | Diff | Notes |
|----------|---------|----------|----------|------|-------|
| Sampler | 0x02009900 | 440B | 272B | -168 | jump tables expand in GCC |
| Producer | 0x0203363c | 140B | 120B | -20 | simpler than original |
| MainUpdater | 0x02033a68 | 188B | 192B | +4 | close match |
| Dispatcher | 0x02033b98 | 436B | 276B | -160 | jump tables expand in GCC |
| ResetEnable | 0x02033d58 | 80B | 84B | +4 | close match |
| Init | 0x02033ddc | 96B | 88B | -8 | close match |

### Why sizes differ

The original code uses **ARM conditional execution** and **inline jump tables**
that GCC does not reproduce at `-O2`:

1. **Jump tables** (`addls pc, pc, rX, lsl #2`): The Sampler and Dispatcher
   use computed-branch tables for pressure scaling (4 cases). GCC emits
   separate compare+branch chains, which are larger.

2. **Conditional execution** (`movne`, `moveq`, etc.): The original uses
   predicated instructions extensively. GCC with `-O2` tends to use
   branch-based conditionals instead.

3. **Literal pool placement**: The original places literal pools immediately
   after function code. GCC aligns them to 4-byte boundaries.

### Logical correctness

All functions implement the correct logic as verified against the Ghidra
decompilation and manual disassembly analysis:

- **Sampler**: Parses touch samples from two sources (shared sample array
  for contact, per-index table for no-contact). Handles pressure scaling
  via a 4-case switch.

- **Producer**: Initializes a dispatcher node with callback, parameters,
  and table pointers.

- **MainUpdater**: Optimized memory copy with 32/4/2/1-byte alignment
  handling via external copy functions.

- **Dispatcher**: Validates input via middleware, allocates buffer for
  touch data, writes size header and marker footer.

- **ResetEnable**: Walks controller linked list, returns first active
  node or 0.

- **Init**: Sets up touch pipeline, returns computed size or -1.

### Files

| File | Address | Function |
|------|---------|----------|
| `src/touch/touch_sampler.c` | 0x02009900 | Sampler_ConsumeSharedSamples |
| `src/touch/touch_producer.c` | 0x0203363c | TouchState_Producer |
| `src/touch/touch_main.c` | 0x02033a68 | TouchState_MainUpdater |
| `src/touch/touch_dispatch.c` | 0x02033b98 | TouchState_Dispatcher |
| `src/touch/touch_reset.c` | 0x02033d58 | TouchState_ResetEnable |
| `src/touch/touch_init.c` | 0x02033ddc | TouchState_Init |
