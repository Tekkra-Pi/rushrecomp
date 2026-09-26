# Touch Subsystem Topic

## Overview
The touch subsystem handles touchscreen input processing, from raw hardware sampling through edge detection and AABB hit-testing for menu interactions.

## Architecture

### Touch State Structure (0x022b65b4, 0x28 bytes)
The main touch state containing coordinate history and edge flags.

| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| +0x00 | 4 | enable | Bit0 = touch input enabled |
| +0x04 | 4 | alternate | Alternate updater mode |
| +0x08 | 4 | x | Current X coordinate (0xffffffff = no contact) |
| +0x0c | 4 | y | Current Y coordinate (0xffffffff = no contact) |
| +0x10 | 4 | flags | Status flags (see below) |
| +0x14 | 4 | history_count | Number of history entries |
| +0x16+ | var | history | Coordinate-history FIFO (8 bytes each) |

### Touch Flags (+0x10)
| Bit | Mask | Name | Description |
|-----|------|------|-------------|
| 0 | 0x01 | VALID | Sample is valid |
| 1 | 0x02 | CONTACT | Touch contact detected |
| 4 | 0x10 | TOUCHED | Current-frame contact |
| 5 | 0x20 | HELD | Held/previous contact |
| 6 | 0x40 | PRESS_EDGE | Rising edge (press) |
| 7 | 0x80 | RELEASE_EDGE | Falling edge (release) |
| 8 | 0x100 | OUT_OF_RANGE | Sample error |

## Function Chain

### 1. Sampler (0x02009900, 440B)
Parses raw packed samples from input subsystem into 12-byte SamplerOutput.

### 2. Producer (0x0203363c, 140B)
Processes sampler output and updates touch state.

### 3. Main Updater (0x02033a68, 188B)
Per-frame touch state update and edge detection.

### 4. Dispatcher (0x02033b98, 0x1b4 bytes)
Routes touch events to registered handlers.

### 5. Reset Enable (0x02033d58, 80B)
Resets touch enable state.

### 6. Init (0x02033ddc, 96B)
Initializes touch subsystem.

## Integration Points
- **Input Subsystem**: Reads from input subsystem at 0x22503e0
- **Menu System**: AABB hit-testing at 0x022c678c
- **Title Menu**: Consumes touch state for menu navigation

## Source Files
- `include/touch_state.h` - Struct definitions
- `src/touch/*.c` - Implementation files
- `src/touch/MATCHING.md` - Build verification notes

## Evidence Confidence
- **CONFIRMED-STATIC**: All struct layouts and function addresses verified through static analysis and Ghidra decompilation
- **CONFIRMED-RUNTIME**: Touch flag semantics verified through runtime testing

## References
- re-analysis.md lines 867-921
- p0-7-summary.md
- touch-bit-analysis.md
