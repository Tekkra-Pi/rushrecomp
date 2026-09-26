# rushrecomp — Sonic Rush Static Recompilation

Static recompilation of *Sonic Rush* (Nintendo DS) to native code: ARM9/ARM7
machine code is recompiled ahead-of-time to portable C via
[mstan's ndsrecomp](https://github.com/RetroPortingToolKit/ndsrecomp), plus a
parallel manual x86-32 recompilation path built from decompiled subsystems.
The recompiled game boots through the full intro and runs in-game stages.

> Status and history: [`RECOMP_PROGRESS.md`](RECOMP_PROGRESS.md),
> [`RECOMP_STATUS.md`](RECOMP_STATUS.md), and [`docs/HANDOFF.md`](docs/HANDOFF.md).
> [`docs/re-analysis.md`](docs/re-analysis.md) is the chronological analysis notebook.

## What's in here

```
rushrecomp/
├── ndsrecomp_title/   # ndsrecomp pipeline: TOML configs (entry points,
│                      #   jump tables, data ranges) for both CPUs
│                      #   (generated/ output and extracted images/*.bin
│                      #    are NOT included — regenerate, see below)
├── patches/           # Required fix for ndsrecomp (see "The finder patch")
├── src/, pc/          # Manual decompilation + x86-32 recomp runtime (Makefile.pc)
├── docs/              # Architecture notes, analysis, emulator setup
├── scripts/           # Extraction / verification / test automation
├── tools/             # ARMIPS, hdrv (headless DeSmuME driver) sources
└── RECOMP_*.md        # Recomp project status documents
```

## Requirements

- A **Sonic Rush ROM you own**, placed at
  `games/Sonic Rush (USA) (En,Ja,Fr,De,Es,It).nds` (or set `SONIC_RUSH_ROM`).
  No ROM, extracted images, or BIOS dumps ship with this repository.
- A built `nds_recompile` + `nds_runner` from ndsrecomp.
- NDS BIOS dumps are optional — the runner's `--freebios --generated-firmware`
  path works for development; retail BIOS dumps can be supplied later.

## The finder patch (required)

The upstream function finder admits unaligned ARM function roots, which splits
real functions mid-instruction; the dispatcher's ARM target masking (`& ~3`)
then rounds fallthrough links back into the previous instruction and the CPU
spins forever (observed at `0x020075D4` during boot). Apply before generating:

```sh
cd path/to/ndsrecomp
git apply patches/ndsrecomp-0001-finder-reject-unaligned-arm-seeds.diff
cmake --build recompiler/build --target nds_recompile
```

## Generating and running

```sh
# 1. Extract arm9.bin/arm7.bin from your ROM into ndsrecomp_title/images/
# 2. Generate the recompiled banks (sharding keeps TU sizes buildable)
nds_recompile --config configs/arm9.toml --bin images/arm9.bin \
  --out generated --bank arm9_main --shards 4
nds_recompile --config configs/arm7.toml --bin images/arm7.bin \
  --out generated --bank arm7_main --shards 2
# 3. Build the runner (banks are picked up by runner/CMakeLists.txt)
cmake --build runner/build --target nds_runner -j2   # -j2: 40-55 MB TUs
# 4. Run
./nds_runner /path/to/bios --freebios --generated-firmware --boot direct \
  --rom "/path/to/Sonic Rush (USA) (En,Ja,Fr,De,Es,It).nds"
```

The manual x86-32 path builds with `make -f Makefile.pc`.

## Tools

- **ndsrecomp** — static ARM/Thumb → C recompiler (RetroPortingToolKit)
- **ARMIPS** — assembly assembler (in `tools/`)
- **hdrv** — headless DeSmuME-based ROM driver (`tools/hdrv.cpp`) for
  reference-behavior verification
- **melonds / Ghidra / radare2** — used during analysis (not bundled)
