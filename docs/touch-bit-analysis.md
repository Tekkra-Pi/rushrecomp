# P0 Task 7: Controlled Touch Bit Testing

## Objective
Name touch bit `0x20`, touch bit `0x100`, fields `+0x0c/+0x0e`, `+0x14`, and `+0x16...` in the touch state structure at `0x22b65b4` using controlled runtime input scripts.

## Background
The touch state structure consists of:
```
+0x00/+0x02  current X/Y (coordinate+1)
+0x04/+0x06  previous X/Y
+0x08/+0x0a  press/accepted X/Y
+0x0c/+0x0e  release/older X/Y history
+0x10        flags: bit0 enabled, bit1 alternate path, bit0x10 current contact,
             bit0x20 previous/held state, bit0x40 press edge, bit0x80 release edge,
             bit0x100 unresolved error
+0x14        configured sample-history count (observed 1)
+0x16...     optional coordinate-history entries
```

## Analysis Plan
1. Create controlled scripts that isolate individual touch interactions (tap, hold, move, release)
2. Capture runtime touch state after each interaction
3. Correlate bit patterns with specific states/interactions
4. Map fields `+0x0c`, `+0x0e`, `+0x14`, and history slots to semantic behavior

## Current Runtime Observations
From `ram_full_1199b.bin` at frame 1199:
```
+0x00-03: ffffffff (idle), ffffffff (idle)
+0x04-07: ffffffff (idle), ffffffff (idle)
+0x08-0b: 0000000000000000
+0x0c-0f: 0000000000000000
+0x10-13: 00000001 (flags: enabled=1, no other bits set)
+0x14-17: 00000001 (count=1)
+0x18-1b: 00000000
```

From `title1199b.csv` touchflags column:
- Consistently shows 0x0001 across frames 300-1140
- Indicates touch is enabled but not actively receiving input

## Next Steps
1. Create gameplay-run + controlled touch script to capture interaction data
2. Extract touch state at key frames around touch events
3. Document bit meanings and field semantics

## Files
- Controlled scripts in `scripts/touch-bit-test-*.txt`
- Runtime data capture in `runtime/`
- Analysis results will be added to `re-analysis.md` under new item 33