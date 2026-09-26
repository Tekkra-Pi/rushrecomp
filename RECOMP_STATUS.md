# Sonic Rush Decompilation - Recompilation Status (2026-09-24)

## Current State

### Build Status
- **516 source files compile clean**
- **0 unresolved symbols** after partial_link
- **108,041 bytes .text** (ARM9 code), 64,024 bytes BSS
- Function coverage: 512/1033 named functions (49.6%)

### Data Tables Extracted (4)
- `g_sin_table` @ 0x0207c040 (256 s16, standard sin/scale 4096)
- `g_accel_scale_table` @ 0x020726d8 (32 s8)
- `g_slope_table` @ 0x0206e470 (128 s16, sin/cos pairs)
- `g_drift_table` @ 0x02077944 (16 s8)

### Function Coverage by Domain
- Stage: 50 functions
- Sound: 42 functions
- Entity: 50 functions
- Display: 31 functions
- Collision: 29 functions
- Player: 16 functions
- And others...

### Stub Status
- 512 functions with implementations (accessors, data access, basic logic)
- 33 empty stubs (all with correct signatures)
- Remaining functions: unknown if ROM address available

## Recompilation Readiness

### What's Ready
1. **Header files** - PhysicsPlayer struct, Types, Constants
2. **Data tables** - 4/4 extracted
3. **Stub layer** - 97 functions with proper implementations
4. **Build system** - Makefile, partial_link works
5. **Symbol table** - function_table.json with addresses, sizes, names

### What's Needed for Recomp
1. **Full function table** - rename all 1033 entries with proper signatures
2. **Overlay configuration** - proper build model for ROM overlays
3. **Vector table** - proper **vector_table address injection into binary**
4. **Main executable** - core ARM9 shell to run the recompiled code

## Immediate Next Steps

1. **Continue naming** - 521 functions still need better names (from 512 currently)
2. **Decompile critical Player functions** - Player_CollisionCheck, Player_Kill, Player_Hurt, etc.
3. **Extract remaining data tables** - g_slope_direction_table, g_bg_data_tables, etc.
4. **Create proper vector table** - for the ARM9 exception vector at 0x02000000
5. **Verify overlay load** - understand how overlays 0x0a3e40 and 0x0593c0 are loaded

## Tech Notes

- ARM946E-S Thumb mode with ColdFire/ARM ISA
- Initial load address: 0x02000000
- Entry point: 0x02000800
- ROM layout: ARM9 @ 0x4000, ARM7 @ 0x8000
- Overlay region: 0x02250000 - 0x022d0000 (partially used)
- Function addresses are physical ROM addresses, not PC-relative

