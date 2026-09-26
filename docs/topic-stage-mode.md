# Stage/Mode System Topic

## Overview
The stage/mode system manages level loading, game flow transitions, and mode state machines. It coordinates between overlay loading, entity creation, and gameplay initialization.

## Key Globals

### Zone State (0x22c4560)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0x00 | 2 | stage_id | Stage ID |
| +0x10 | 2 | mode_word | Mode (0=title, 2=demo, 3=gameplay) |
| +0x14 | 4 | flags | Flags (bit2/4/8 cleared during transitions) |
| +0x18 | 1 | flags_18 | Flags (bit0x40 cleared by mode-entity) |
| +0x1c | 1 | zone_byte_c | Zone byte from stage table |
| +0x1d | 1 | zone_byte_d | Zone byte from stage table |
| +0x20 | 4 | key_checksum | Key checksum |
| +0x24 | 4 | flags_24 | Set on valid key |

### Game-State Context (0x22c4490)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0x04 | 4 | mode_ctx | Mode context |
| +0x48 | 4 | stage_id | Stage ID |
| +0x4c | 1 | stage_byte_c | Stage byte from zone-state |
| +0x4d | 1 | stage_byte_d | Stage byte from zone-state |

### Mode-Transition Data (0x22c4410)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0x00 | 4 | flags | Flags (bit0x40 triggers dispatch) |
| +0x02 | 4 | counter | Counter (decremented toward target) |
| +0x04 | 4 | mode | Mode argument (0x43='C', 0x42='B') |
| +0x06 | 2 | target | Target (0x100 or 0x5000000) |
| +0x10 | 4 | data | Derived value |

## Function Chain

### Mode Transition Initiator (0x02043c7c, 276B)
Allocates a 0x3e00-sized mode entity and initiates stage transitions.

### Fade/Counter Machine (0x02043b78, 220B)
Update callback that decrements counter toward target and triggers mode changes.

### Mode Setup (0x02043abc, 172B)
Reads mode flags and derives display values.

### Mode Dispatcher (0x0204bed0, 76B)
Branches on mode word to select appropriate game flow.

### Terminal Gameplay (0x0204bd94, 68B)
Final gameplay handler that runs the main game loop.

## Stage Object-Loader Table (0x2074184)
- 120 entries, 10 zone rows, 12 act slots per row
- Rows 0-5: Main zones
- Rows 6-9: Special/boss resources

## Per-Stage Table (0x2073bd0, stride 0x22)
| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0x00 | 1 | zone_byte_c | Zone byte C |
| +0x01 | 1 | zone_byte_d | Zone byte D |
| +0x02 | 2 | stage_id | Stage ID |
| +0x04 | 2 | checksum | Checksum / key presence |
| +0x06 | 2 | _pad | Padding |
| +0x0a | var | key_path | "/dat/key/keyXX.bin" path |

## Mode Flow
1. **Mode 0 (Title)**: Title menu and intro sequences
2. **Mode 2 (Demo)**: Attract mode demo playback
3. **Mode 3 (Gameplay)**: Active gameplay with HUD/pause

## Evidence Confidence
- **CONFIRMED-STATIC**: All struct layouts and function addresses verified
- **CONFIRMED-RUNTIME**: Mode transitions verified through runtime testing

## References
- re-analysis.md lines 691-770
