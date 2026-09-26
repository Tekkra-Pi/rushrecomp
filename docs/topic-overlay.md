# Overlay System Topic

## Overview
The NDS overlay system allows code and data to be loaded into fixed RAM regions at runtime. Sonic Rush uses two main overlays: overlay 1 (title/menu) and overlay 0 (gameplay).

## NDS Overlay Architecture

### ROM Layout
- **ARM9**: offset 0x4000, load 0x02000000, size 0x86898
- **ARM7**: offset 0x1880000, load 0x02380000, size 0x26f24
- **Overlay tables**: Located after ARM9/ARM7 sections in ROM

### RAM Regions
- **Fixed ARM9**: 0x02000000 - 0x02086898 (static code)
- **Overlay 1 (Title)**: 0x022c5c80 - 0x0231f01f (loaded at runtime)
- **Overlay 0 (Gameplay)**: 0x02320000 - 0x02350000 (192 KiB)

## Overlay Loader Functions

### Overlay Unload (0x0200c1ec)
- Unloads an overlay from RAM
- Called before loading a different overlay

### Overlay Load (0x0200c240)
- Loads an overlay from ROM into RAM
- Executes initializer list after loading

### Overlay Scheduler (0x0203051c)
- Manages overlay transitions
- Writes to globals:
  - `0x02087cb0`: One-shot callback slot
  - `0x02087cb4`: Active ARM9 overlay ID
  - `0x02087cbc`: Requested ARM9 overlay ID
  - `0x02087cc8`: Overlay loaded flag

## Overlay 1 (Title/Menu)

### Lifetime
- **Load**: Frame 11 (after boot)
- **Unload**: Frame 1267 (transition to gameplay)
- **Range**: 0x022c5c80 - 0x0231f01f

### Key Functions
| Address | Name | Description |
|---------|------|-------------|
| 0x022cfb18 | Post-load initializer | Title setup after overlay load |
| 0x022d8939 | TitleMenu_Update | Live title menu update |
| 0x022d7a84 | TitleMenu_HitTest | AABB hit-testing for menu items |
| 0x022d8798 | TitleMenu_Confirm | Menu item confirmation |
| 0x022c8458 | TitleTransition_TeardownAndSchedule | Shared teardown |
| 0x022d301c | ModeEntity_Constructor | Unreachable dead code (normal play) |

### Dead Code Region
- **Range**: 0x022c6000 - 0x022c9000
- Contains `0x022c7ac8` (TitleMenu_Update dead version)
- Contains `0x022c678c` (TitleMenu_HitTest dead version)
- Contains `0x022c8480` (constructor with zero callers)

## Overlay 0 (Gameplay)

### Lifetime
- **Load**: Frame 1267 (after title unload)
- **Active**: Frames 1267 - end
- **Range**: 0x02320000 - 0x02350000 (192 KiB)

### Key Functions
| Address | Name | Description |
|---------|------|-------------|
| 0x023375f8 | Zone/camera exchange | Exchanges zone/camera values |
| 0x02337618 | Zone/camera exchange | Exchanges zone/camera values |
| 0x02337634 | Zone/camera exchange | Exchanges zone/camera values |
| 0x02336050 | Zone/camera exchange | Exchanges zone/camera values |
| 0x0232ecac | Sound/effect helper | Sound processing |
| 0x0232ee24 | Unknown | Gameplay helper |
| 0x0232f234 | Unknown | Gameplay helper |
| 0x0232f3ec | Unknown | Gameplay helper |

## Overlay Transition Flow

### Title to Gameplay
1. Frame 1261: User selects "GAMEPLAY"
2. Frame 1266: `0x0203051c(0, 0x0204d834)` called
3. Frame 1267: Overlay 1 unloaded, Overlay 0 loaded
4. Frame 1267+: Gameplay initializes via `0x0204d834` -> `0x0204d6d0(3)`

### State Machine
```
Boot -> Overlay1_Load -> Title_Menu -> User_Selection
    -> Overlay1_Unload -> Overlay0_Load -> Gameplay_Init -> Gameplay_Loop
```

## Globals

### Overlay State
| Address | Name | Description |
|---------|------|-------------|
| 0x02087cb0 | One-shot callback | Post-load callback slot |
| 0x02087cb4 | Active overlay ID | Currently loaded overlay |
| 0x02087cbc | Requested overlay ID | Requested overlay to load |
| 0x02087cc8 | Loaded flag | Overlay loaded status |

### Fixed ARM9 Stubs
- Range: 0x02054e00 - 0x02054fec
- Allows cross-overlay function calls
- Exposes: 0x022d3011, 0x022d301d (overlay 1 functions)

## Evidence Confidence
- **CONFIRMED-STATIC**: Overlay loader functions (0x0200c1ec, 0x0200c240)
- **CONFIRMED-RUNTIME**: Overlay transitions verified via hdrv frame captures
- **CONFIRMED-RUNTIME**: Dead code region identified through caller analysis

## References
- re-analysis.md lines 552-790
