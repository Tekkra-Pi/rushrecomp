# Technical Notes

## Setup Checklist

- [ ] Download melonds to ${HOME}/tools/melonds or configure path
- [ ] Download Ghidra (latest release) to ${HOME}/tools/ghidra_*
- [ ] Place Sonic Rush ROM in games/sonic-rush.nds (or set SONIC_RUSH_ROM env var)
- [ ] Verify ARMIPS is working

## Known Issues

## Development Workflow

### Breaking Down a Function
1. Identify function entry point from prologue analysis
2. Map basic blocks and control flow
3. Identify patterns (loops, state machines, switch cases)
4. Extract relevant API calls
5. Generate struct definitions when possible

### Documentation Standards
- Record memory addresses and offsets
- Note observed behavior vs. expectations
- Keep track of assembler rebuilds
- Maintain instruction-matching friendly decompilation

## Notes