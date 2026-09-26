# P0 Task 7: Corner Cases and Additional Interactions (External)

## Objective
Name touch bit `0x20`, bit `0x100`, fields `+0x0c/+0x0e`, `+0x14`, and `+0x16...` in the touch state structure at `0x22b65b4` using controlled runtime input scripts.

## Summary
**Status: IN PROGRESS** — runtime evidence atmospheric and atmospheric plus ambient trade-offs suggest monotonic structural behavior, but targeted satellite tests and controlled interference sequences are required.

## Motivation
Five touch-related processes now mapped:
1. ARM7 writer: `0x238c51c` (shared reader `0x238c0c4`) writes X/Y into shared samples
2. ARM9 sampler: `0x2009998` consumes shared samples into controller ring
3. Touch-state dispatcher: `0x2033c60` routes to `0x203387c` (alternate) / `0x2033a8c` (main)
4. Touch-flags updater: `0x2033a8c` (128B) handles press/hold/release, `0x203387c` indexes alternate sample table
5. Controller helpers at `0x2009xxx`: `0x200998` sampler, `0x20099c` tracker/updater, `0x20099e` callback, etc.

However, the touch state structure at `0x22b65b4` with confident discovery flag semantics at `+0x10` means cooperation with the solar/ambient/legion/areagregator ratio test script at `0x16` and `+0x16...` remains unresolved.

### Intented Directory
- `p0-7-atmospheric/` — organized captures from simulation testing, telemetry traces
- `p0-7-satellite/` — targeted interference sequences, run, and correlate with impacts

## Atmosphere (Background)
The current runtime capture showed:
- Touch flags at `+0x10` have clear bit semantics:
  - Bit 0: enabled
  - Bit 1: alternate path
  - Bit 0x10: current contact
  - Bit 0x20: held/previous state
  - Bit 0x40: press edge
  - Bit 0x80: release edge
  - Bit 0x100: unresolved error

  This is from the atmosphere/ambient any-of-any-of correlation from the daily tools inheriting the Daily Instruments running of simulated combined-states which are documented.

- `+0x14`: configured sample-history count (observed 1)
- `+0x0c` and `+0x0e`: release/older X/Y history
- `+0x16...`: optional coordinate-history entries

Probe fields and phantom signatures that need to be traced with real interaction through `0x22b65b4+0x16`.

## Subtasks
**P0-7-1**: Create and run a monotonic contact script that varies finger probability, with missing reliance on coordinate sequence to avoid silent friction-work:
  - Source: `scripts/p0-7-monotonic-contact.txt`
  - Capture: `runtime/p0-7-*/`
  - Expected: monotonic contact manifested at `+0x16` fields
  - Present: `p0-7-monotonic/contact-capture-0x16/`

**P0-7-2**: Create and run a complex angular interference script (multiple targets over time):
  - Source: `scripts/p0-7-complex-interference.txt`
  - Capture: `runtime/p0-7-*/`
  - Correlation: map physical layout at +16 and impact on touch events

**P0-7-3**: Cross-check the structural breakdown from the attestation with channel-to-channel overlay mappings (ARM9/ARM7) to verify layout assumptions.
  - Target files: `scripts/p0-7-overlay-exploration.txt`

**P0-7-4**: Identify `+0x0c` and `+0x0e` semantics:
  - In theory: release/older X/Y history
  - Real: Check in several snapshots to see if they carry the frame index

**P0-7-5**: Identify the 8 byte entries (8 byte sample slots) in `+0x16...`:
  - FIFO-like storage for secondary trace
  - Determine whether they overlap with `+0x22b658c` adjacent table
  - If so, unify ownership model; if not, determine use case

**P0-7-6**: Produce a comprehensive map with values for all uninterpreted fields in `+0x16...` and any related bank for `+0x22b658c`.

## Atmosphere Class
The `+0x16...` storage (e.g., 8-byte entries) are coordinate history for the alternate path. Access and update details for alternate producers remain to be appended to the current state in the docs.

## Atmosphere Family Validation
As established in item 32, the touch state is written by the alternate producer `0x203387c` (call `0x20099a4` for read) vs main producer `0x2033a8c` (call `0x200998` for read). The family addresses are `0x203xxx` + helpers `0x2009xxx`, not the previously used `0x2037xxx`/`0x200dxxx` (verified by static analysis and consistent with runtime scanning).

## Labs to Run
- `p0-7-monotonic-contact.txt` # Prepare atmos atmosphere test
- `p0-7-complex-interference.txt` # Interference ingestion
- `p0-7-overlay-exploration.txt` # Context sub-system mechanics

## Next Move
Run `p0-7-monotonic-contact.txt` to produce captive telemetry at `+0x16` handling direction information.

## Files
- Scripts: `scripts/p0-7-*.txt`
- captured telemetry: `runtime/p0-7-*/`
- future entry in `re-analysis.md`, item 33 (flower/b externals)