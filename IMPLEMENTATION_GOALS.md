# Sonic Rush Decompilation - Recomp Implementation Goals

## High Priority (Can accelerate recompilation)

### 1. Player System Functions
**Status:** 16/50 Player_* functions have implementations
**Action:** Focus decompilation on remaining Player_* functions
- Player_DeathSequence @ ? 
- Player_Hurt @ ?
- Player_Kill @ ?
- Player_IsUnderCeiling @ 0x020c7048 (needs bin extraction)

### 2. Collision System
**Status:** 29/110 Collision_* functions implemented
**Action:** Implement core collision functions
- Collision_SecondaryCheck @ ? (crucial for gameplay)
- Collision_TileTest @ ? (core tile engine)

### 3. Display/Graphics
**Status:** 10/60 Display_* implemented
**Action:** Implement display functions needed for rendering
- Display_FadeFromBlack @ ?
- Display_FillRect (register-based, easy)
- Display_SetFog (register-based, easy)

### 4. Stage/Level
**Status:** 50/100 Stage_* functions implemented
**Action:** Implement chunk loading and collision
- StageChunk_Load @ ?
- StageCollision_* (floor/ceiling/wall) @ ?

## Medium Priority

### 5. Overlay Manager
**Status:** Unknown - need to understand how:
- Overlays are invoked/loaded
- Overlay entry points work
- Overlap with main ARM9 code

### 6. Data Tables
**Status:** 4/30+ tables extracted
**Action:** Extract remaining tables
- g_slope_direction_table (referenced in player_movement.c)
- g_bg_data_tables
- g_object_data_tables

### 7. Main Loop/Binaries
**Status:** Need ARM9 shell
**Action:** Build a minimal ARM9 executable that:
- Sets up hardware
- Loads overlays
- Calls main(gameplay_entity_init)

## Practical Recompilation Strategy (No ndsrecomp needed)

Even without ndsrecomp's decompilation pipeline, we can:

1. **Semi-functional recomp:** Use current stubs + extracted data + basic build
   - 108KB .text is a starting point
   - 64KB BSS is game state footprint
   - Many functions work with stubs that just return default/zero values

2. **Incremental decompilation:** Focus on critical gameplay paths
   - Start with Player initialization, collision, movement
   - Add Stage collision/chunk loading
   - Add display rendering functions

3. **Partial binary:** Don't need 100% coverage
   - Works in stages as functions are added
   - Can test gameplay with stubbed collision/physics
   - Validate algorithm correctness even if stubs replace full logic

