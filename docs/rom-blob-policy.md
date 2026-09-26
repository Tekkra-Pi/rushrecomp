# ROM-Derived Blob Policy

## Overview
This document defines the policy for committing ROM-derived artifacts to the repository.

## Allowed Artifacts

### Committed (safe to version control)
- **`.bin` files**: Raw function bytes extracted from ROM (ARM9 code only)
- **`.asm` files**: ARMIPS reassembly harnesses (generated, contain no ROM data)
- **`.txt` files**: Ghidra decompilation output, disassembly text
- **Symbol databases**: Address-to-name mappings, struct definitions

### Not Committed (ROM-derived, use .gitignore)
- **`.patched.nds` files**: Generated patched ROMs
- **RAM dumps**: Full memory snapshots from emulators
- **Overlay blobs**: Runtime-captured overlay data (should be re-extracted from ROM)

## Rationale
- `.bin` files are small (typically 50-2000 bytes each) and contain only code, not assets
- They enable verification without requiring the ROM for every build
- They are necessary for the byte-matching acceptance criterion in `src/touch/`

## Requirements
1. **Never commit the ROM itself** (`*.nds`)
2. **Never commit full RAM dumps** (`*.raw`, `*.dmp`)
3. **Always use `SONIC_RUSH_ROM` env var** for ROM path
4. **Document any new ROM-derived artifact types** in this file

## Verification
Run `scripts/verify-rom.sh` to verify ROM integrity and extraction consistency.

## Exceptions
If a large ROM-derived artifact is necessary for testing or CI, document the exception here and ensure it is added to `.gitignore` after use.
