# Emulator Setup

## melonds

### Linux Installation

1. Download the latest release from: https://github.com/melonDS-emu/melonDS/releases
2. Extract to: ${HOME}/tools/melonds
3. Available executables:
   - `melonds` - Main emulator binary
   - `melondsDebugger` - Debugging version

### Windows Installation

1. Download from GitHub releases
2. Extract to: ${HOME}/emulators\melonds
3. Use melonds.exe as emulator

### Configuration

Configure in `config.ini`:
- ROM path: `path=games/sonic-rush.nds` (or set `SONIC_RUSH_ROM` env var)
- FPS override if needed
- Debug settings

## Ghidra Setup

1. Download Ghidra from: https://ghidra-sre.org/Download.html
2. Extract to: ${HOME}/tools/ghidra_*
3. Run `ghidraRun`

### Project Creation

- New Project → Non-Shared Project
- Target: `games/sonic-rush.nds`
- Extractors: Use default

### Decompilation Workflow

1. Import binary with default analysis
2. Identify executable regions (.text, .data)
3. Start with main and critical game systems
4. Save custom scripts to ${HOME}/.ghidra/Scripts

## Next Steps

1. Install emulators to tools/ directory
2. Place Sonic Rush ROM in games/ directory
3. Create functional scripts directory structure
