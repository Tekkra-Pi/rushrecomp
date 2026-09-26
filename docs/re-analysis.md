# Sonic Rush RE Analysis

## Game Identification
- Title: "SONIC RUSH" (code ASCE8P) at ROM offset 0
- Both playable characters confirmed: SONIC and BLAZE (subtitle strings)
- US version, multi-language (ENG/FRA/DEU/ITA/SPA/JPN strings present)

## ROM Layout
- ARM9 section: 0x02000000 - 0x02086898 (551064 bytes); addresses above this are loaded overlays/data
- ARM9 firmware entry: 0x02000800 (entry0)
- ARM7 binary: 0x02380000 (entry1)
- 2337 functions identified by radare2 analysis

## Initialization Chain
- fcn.02000afc -> fcn.0202fcdc -> fcn.0202fe00 -> fcn.0200e098
- fcn.0200e098: main game initialization (532 bytes)
  - Sets up hardware registers (0x4000208, 0x4001000)
  - Enables subsystems via flags (bit 3 = ?, bit 4 = ?)
  - Calls fcn.02002b24 (hardware config), fcn.02002b70
- fcn.02030598: game system init (508 bytes)
  - Initializes all subsystems in sequence
  - Calls display init, sound init, physics init

## Hardware Access Functions (270 total)
Detected by scanning all disassembly for 0x4000xxx references.

### Display/Graphics
- fcn.02004398: display init (0x4000504 serial, 0x4000350-0x400035c sound regs, 0x4000060 GPIO)
- fcn.02002c10: backlight/display config (0x4000304, 0x4000004, 0x4000008)
- fcn.0200831c: memset/memcpy helper (144 callers)
- fcn.02008360: memory block copy

### Sound
- fcn.02006f80, fcn.0200701c: sound device init
- fcn.0200f160: audio mixer
- fcn.02005348, fcn.020053c8: IRQ/register wrappers (0x4000208, 0x4000210)

### Input
- fcn.02005520: button/key matrix handling (0x4000020 area, button table at 0x207d01c)
- fcn.020082e8: input polling

## Core Game Systems (from caller-count + size ranking)

### Player Character System
- fcn.020657dc (1740B): **player physics/movement**
  - Reads X/Y pos at entity+0x10, +0x14 (8:8 fixed point)
  - Facing direction at byte+1
  - Direction handling with `rsb` (negation for left)
  - Called from fcn.02065ee0
- fcn.02065ee0 (184B): **character controller/state driver**
  - Bitflag manipulation at entity+0x58
  - Flags: 0x400000 (ground?), 0x1, 0x400, 0x800
  - Routes to physics (020657dc) or state machine (020648c4)
- fcn.020648c4 (3416B): **player state machine**
  - Reads entity fields (0x80, 0x10, 0x14, 0x58)
  - 4-case switch on direction/orientation
  - Called via function pointer (dispatch table)

### Entity/Actor System
- fcn.0204bf88 -> fcn.0205f25c -> fcn.0205e378 chain
- Small 16-24 byte wrappers = per-actor callbacks
- Base entity struct at 0x22b4574
- Jump tables at 0x204bb44

## Asset/Level Data (from izz strings)
- Zone abbreviations: dm_bbz, dm_ssn, dm_blz, lz7, k_bg
- 7 zones confirmed (z1s-z7s sound data)
- Act types: ex00-ex04, hit00/hit01, id_ex00, idou (camera)
- Bosses: dm_eak_bbz, dm_eak_boss, dm_ex_boss
- Character scenes: pdm_meet1, dm_meet2_son/blz
- Models: /mod/bs4_enemy.nsbmd, /mod/bs2_water.nsbmd
- Asset registry table at 0x2076974 (pointer + flags pairs)

## Automation Scripts
- scan.sh: lists functions referencing DS hardware (270)
- callers.sh: call-count ranking
- rank.sh: combined size + callers ranking
- Output: hw_funcs.txt, 0x0*.txt (function dumps)

## Collision System (fully mapped)
Call chain: 020657dc -> 02066010 -> {02066638, 02067670} -> 020670bc -> 02066bc0

### fcn.020657dc (1740B) - Movement + collision resolution
- Reads X/Y pos at entity+0x10/+0x14 (8:8 fixed point)
- Direction index from facing byte+1: `(dir+0x20) & 0xc0 >> 6` (0-3 = R/D/U/L)
- Probes both axes via 02066010, resolves shallowest penetration (`movge`)
- On hit: set flag bit2 at +0x58, clear velocity by direction
- Axis-separated slide: switch on dir does sl+=/r4-= etc.
- Writes resolved position back (fixed-point preserve)

### fcn.02066010 (160B) - Collision dispatcher
- Flag 0x1000 clear -> probe tiles via 02066638
- Flag 0x200 clear -> probe entities via 02067670
- Returns min of both (movgt)

### fcn.02066638 (1112B) - Tile raycast
- Camera bounds struct at 0x22c4dcc (left/top/right/bottom at +0/+4/+8/+0xc)
- Flag 0x20 -> camera clamp path (clamp [-0x1e, 0x1f])
- Else: step through tiles in 8px increments (r8=8, sb+=8), blx to 020660f4/02066360
- Returns pixels of clearance (0 = solid, positive = free)
- Tile offset from `r6 & 7` (position mod 8)

### fcn.02067670 (936B) - Entity AABB sweep
- Entity list at 0x22c4e64, count at 0x22c4ddc
- Iterates active entities, skips self/null
- AABB test: X from [sl+0x10]>>8 - 0x18 to +[sl+0x20]*8, Y similarly
- Hitbox half-size at +0x20/+0x22, minus 0x18 margin
- On hit: call 020670bc, link entities ([sl+8]=self, [r7+0x80]=other)
- Returns collision depth/response

### fcn.020670bc (1084B) - Sloped surface response
- Loads fn ptr 02066bc0 (slope handler) from literal pool
- Step size r4 = -7 (bit0 set) or 8, inverted by axis
- +0x10 offset in return paths (height mask offset)
- Returns slope clearance distance

## Main Game Loop (FOUND)
`fcn.02000b24` (348B) at 0x02000b24 is the infinite main loop, entered from entry0 via `bx r1` at 0x20008d8 (never returns). Structural:

```
fcn.02000b24 (MAIN LOOP):
  while(1) {
    fcn.020105d0 / fcn.0202ff74        ; init/state checks (branch)
    input = (keys @ 0x27fffa8 | hw 0x4000130) ^ 0x2fff
    fcn.02035134(input)                ; input processing
    fcn.02033c60                       ; touchscreen history/edge updater
      (touch state @ 0x22b65b4: bit1 -> 0203387c, else 02033a8c)
    fcn.02034d34                       ; subsystem
    fcn.0202fd94                       ; boot/reset state machine (@0x2087ccc)
      state0: 0200e098(0xc,0,0) main init
      state1: 0200df88(0)
      state2: loop 0200df88(1) then 0200e3e4(2,0)
    fcn.0202ff80
    fcn.020302d8                       ; ARM9 overlay-ID transition + one-shot callback
    fcn.02036fb8 (trampoline) -> fcn.02036e04
      ; PER-FRAME ENTITY UPDATE WALKER
      ; walks linked list @ 0x22b4574, calls blx [ent+8] (update fn ptr)
    fcn.02034114
    fcn.02036a0c(0), fcn.02036a0c(1)   ; object-list iteration (32 slots @0x2072c78)
      ; walks +0xae next-chain, calls 02008300 (halfword copy), 020082e8 (input poll)
    power/vblank: 0x4000304, 0x4000540
    fcn.02034ec0 (if flag 0x100)       ; eeprom/flash?
    fcn.0203005c                       ; sync hw settings from 0x208922c
    sleep (swi 0x03)
  }
```

### Boot chain (entry0 @ 0x2000800)
- Sets up SVC/IRQ/SYS stacks, copies code, clears BSS (0x27e0000-0x1ff8000)
- entry0 -> fcn.0202fcc8 -> fcn.02000afc (init chain) -> fcn.0202fcdc
- Final `bx r1` (r1 = 0x02000b24) enters the main loop

### Player per-frame update path (end-to-end)
```
fcn.02036e04 (entity walker) --blx [ent+8]--> fcn.0206c3d0 (player update)
  -> fcn.0206bd5c (movement iterator)
       steps entity toward target (+0x1c/+0x20) in 0x800 increments
       per step: fcn.02065f98 (trampoline) -> fcn.02065ee0 (controller)
          -> fcn.020657dc (physics + collision)
             -> fcn.02065624 (probe) / fcn.02066010 (dispatcher)
                -> 02066638 (tiles) / 02067670 (entities) / 020670bc (slopes)
```
- Player update callback `fcn.0206c3d0` registered at entity spawn in `fcn.0204da44`
  (also loads "_act_ac_fix_gameover.bac" -> game-over sequence)
- `fcn.0206c3d0`: reads player @0x22b4574, +0x54 flags (bit2=busy, bit3->4, bit0x10),
  AABB vs fcn.02068e58, then 0206bd5c movement
- `fcn.0206bd5c` (408B): iterative step loop; saves +0x1c/+0x20 targets, steps
  X/Y by 0x800 toward target, runs controller each step, re-checking flags
- `fcn.02036a0c` (200B): object-list manager for arg 0/1 lists (table @0x2072c78)

## Input System (keyboard state flow)
Hardware poll: `keys = (KEYCNT @ 0x27fffa8 | KEYINPUT @ 0x4000130) ^ 0x2fff & 0x2fff`
- Polled in main loop (fcn.02000b24 @ 0x2000b7c-0x2000ba0) and fcn.020352a8 (raw poll helper)

### fcn.02035134 (164B) - input debounce / edge detection
Layout of input struct (base 0x22503e0):
```
+0x00  current  keys this frame (polled)
+0x02  previous keys last frame
+0x04  pressed  edge (current & ~prev)
+0x06  released edge (~current & prev)
+0x08  held     keys past auto-repeat delay
+0x0a+bit*2  running repeat timers (12 bits)
+0x22+bit*2  repeat delay config (set by fcn.020351d8)
+0x3a+bit*2  reload values
```
- Initialized by fcn.020351d8 (default delay 0x14 = 20 frames; or config table if arg)
- Loop over 12 key bits; decrements timer; when 0, sets held bit and reloads

### Input consumers (all reference literal 0x22503e0)
- fcn.02067ddc: scripted attract/demo sequence controller (504B, verified; spawned by
  fcn.02068094). Its 0x20-byte data block is:
  ```
  +0x00 flags: bit0 = reset sequence on next active tick; bit1 = script active
  +0x04 total frames elapsed
  +0x08 current script-step index
  +0x0c frames elapsed in current step
  +0x0e last global sequence mode
  +0x10 8-frame transition countdown
  +0x14 spawner argument / sequence selector
  +0x18 pointer to script entries (4-byte stride: halfword input mask, byte duration)
  +0x1c loaded script/resource handle
  ```
  It watches the halfword at `0x22c4ee4`. Modes 0–2 clear global `0x2087cb8` bit
  `0x40`; modes 3/5 set it and arm an 8-frame transition; mode 4 requests a one-shot
  script reset; modes 6+ destroy the owning entity. While active, it passes each
  entry's halfword mask to `fcn.02035134`, advances when the entry duration byte
  expires, and sets mode 1 when it reaches a zero-duration terminator. This is
  scripted keypad input—not touch input.
- fcn.02068094 (240B, verified): allocates the `0x20` controller entity through
  `fcn.020372d0` (update `02067ddc`, type 1). It clears the data block, saves arg0
  at `+0x14`, and returns early when arg1 is null. Otherwise it marks the script
  active, sets `0x22c4ee4 = 2`, loads arg1 via `fcn.02033fbc`, stores its first word
  at `0x208fe70`, and points `+0x18` just after that word.
- fcn.020556xx: copies input (current/pressed/held) into menu UI entity +0x174/0x176/0x178
- fcn.02059dd4, fcn.0205ec94: Start (bit3) / A,B (bit0,1) skip/confirm handlers
- fcn.0206d044: text/cheat input (key->ASCII via table 0x207cfb8, buffer at 0x224ffa0)
- Headless title navigation is now reproducible with `scripts/title-to-gameplay.txt`:
  keypad Start at frames 800–809 reaches the menu, then a tap at bottom-screen
  `(128, 60)` selects **GAMEPLAY** and reaches the Leaf Storm Zone 1 Act 1 intro
  by frame 1300. `hdrv` parses script values with `%lx`, so coordinates must be
  explicit hexadecimal (`0x80 0x3c`); bare `128 60` means hex `0x128,0x60`
  (296, 96) and taps off-screen.

### Touchscreen handoff (ARM7 -> ARM9; verified boundary)
A held headless tap makes only these shared high-RAM input bytes differ from an
otherwise identical no-touch run:
```
0x027fffaa  touch X in 12.4 fixed point: x=128 -> 0x0800; x=64 -> 0x0400
0x027fffac  packed Y/status: y=60 -> 0xc13c; y=128 -> 0xc180;
              idle sample observed as 0xc600
```
The ARM7-side writer is now identified (re-analysis item 32): fixed-ARM7
`0x238c51c` (and the case-0 touch handler `0x238c594`) writes X to
`0x027fffaa` and packed Y to `0x027fffac`; the shared touchscreen sample reader
is `0x238c0c4`. The static ARM9 family
`0x2033c60 -> {0x203387c,0x2033a8c}` (dispatcher, alternate/main) plus the
sampler `0x2009998` converts those shared samples into the 0x28-byte
history/edge state at `0x22b65b4`. A `(128,60)` tap produces packed
halfwords `(129,61)` (`0x003d0081`) because this layer stores coordinates plus
one; idle coordinates are `0xffff`. Runtime flags at `+0x10` transition:
`0x01` idle -> `0x51` press -> `0x31` hold -> `0xa1` release -> `0x01` idle.
Static behavior therefore confirms bit `0x10` as current-contact/internal active,
`0x40` as press edge, and `0x80` as release edge; bit `0x20` tracks the prior/
held state. Bit `0x100` is the out-of-range/error condition raised by
`0x2033a8c` when the sampled Y halfword is nonzero/out-of-range.

The loaded title-menu update at `0x022c7ac8` directly reads `0x22b65b4+0x10`,
uses `+0x08/+0x0a` when bit `0x40` is set, and calls AABB hit-test
`0x022c678c` on its menu item at entity-data `+0x5b4`. Return value 1 takes the
same confirm path as Start pressed (`0x22503e0+4 & 8`). Focused runtime blobs and
disassemblies are preserved as `runtime/title_menu_update_022c7ac8*` and
`runtime/title_hit_test_022c678c*`.

The earlier dynamic-task-callback theory is disproved: during the title run,
`0x207f02c+0` remains static no-op `0x02037c5c` and `0x207f02c+0x10` remains
NULL. `0200d998` is still an IRQ6 hardware-adjacent controller, but it does not
forward the title tap to a dynamically loaded callback.

### Title transition after confirmation (mapped through next entity creation)
The loaded callback `0x022c8458` is a 0x28-byte shared teardown/scheduling
wrapper, not the full transition state. It removes every static entity with
container marker `0x1173` via `02036fdc`, disables touch through `02033d40`,
then reaches `0203051c(1, 0x022d301d)` through trampoline `02054ed8`.
References at `0x022c6d48`, `0x022c742c`, and `0x022c7d28` show that multiple
title/demo branches converge on this same callback.

`0203051c` writes requested ARM9 overlay ID `0x02087cbc` and a one-shot callback
at `0x02087cb0`. The main-loop function `020302d8` compares requested ID with
active ID `0x02087cb4`; when they differ it synchronously unloads the active
ARM9 overlay through `0200c1ec(0,id)`, loads the requested one through
`0200c240(0,id)`, updates loaded flag `0x02087cc8`, then `blx` calls the queued
callback and clears only `0x02087cb0`. The helper implementations match the
NitroSDK overlay APIs. Here the requested ID is 1.

Thumb callback `0x022d301d` (code at even `0x022d301c`) tests
`[0x022c4560]`: null dispatches argument 0 and non-null dispatches argument 1 to
constructor `0x022d2e3c`. That constructor is 0x162 bytes and allocates a
0x150c-byte entity data block through `020372d0`, with update `0x022d2461`,
destroy `0x022d2cbd`, type `0x5000`, and marker `0x1173`. It clears the block,
stores it at `0x0231f050`, enables/reset touch via `02033d60`, and selects
state callback `0x022d13f5`, `0x022d0d35`, or NULL for constructor modes 0, 1,
or 2 respectively. It creates a child entity with update `0x022d224d`, type
`0xf100`, marker `0x1173`, initializes eight embedded blocks, and configures
both display engines.

Adjacent Thumb helper `0x022d3044..0x022d3110` clears object-list/display
headers and zeroes main/sub BG VRAM plus all four palette regions. It is not
called or fallen into by `0x022d301c`; participation in this transition remains
unproven. Focused binaries and relative disassemblies are preserved under
`runtime/title_transition_*`, `runtime/title_next_mode_*`,
`runtime/title_mode_entity_*`, and `runtime/title_display_reset_*`.

### Player movement input (keys -> velocity)
```
player update 0206c3d0
  -> fcn.0206919c   : velocity injection into raw input +0x36/+0x38/+0x3a
       three binding slots, all gated by the 0x4000000 flag on both sides:
       * +0x80  primary object binding (springs/platforms):
           - bound obj +0x58 has bit8(0x100) (spawning): adds obj +0x28/+0x2a/+0x2c
             to +0x36/+0x38/+0x3a  (= spring/launch impulse)
           - else: adds position delta (obj+0x10 - obj+0x1c, ...) = platform carry
           - plus +0x2a low-byte decay logic toward 0x100
       * +0x84  partner binding: adds position delta + partner +0x28 velocity
           with 0x100 wrap decay; cleared when partner busy
       * +0x130 mover-controller binding: ctl struct has +4 = target entity,
           +8 = source entity. If source != target and source has 0x4000000:
           adds source +0x28 (clamped to source +0x48 then player +0x48) to +0x36.
           +4/+8 slots cleared when the bound entity's +0x54 busy bit(2) is set.
           NOTE: NOT the dpad source. Written only by runtime mover/3D-model
           object code (no ARM9 player-path writer exists; only str hits are the
           3D-model entity in fcn.0201fcc4 @0x201fe04 and a stack arg @0x2030f8c).
  -> fcn.020694b0   : movement/accel handler (856B)
       reads raw input +0x36/+0x38/+0x3a (zeroed by player update at frame end)
       fcn.020693f0 : rotate input vector by ground angle
         (uses slope table 0x206e470, matrix rot via 02000e98/02000eb0)
       scale constant aav.0x0207cf7c = 256 (0x100)
       writes +0x28 = X, +0x2a = Y, +0x2c = Z input velocity
  -> physics 020657dc / state machine 020648c4 read +0x28/+0x2a
```
- dpad source resolved: dpad -> 020694b0 (via +0x36) -> +0x28. The +0x130 field is a
  mover binding only; it is NOT how dpad input reaches the player.

## Player / Entity Struct Layout
Player entity struct size 0x134 (fcn.0204da44: `mov r2, 0x134`; clear via 020082e8).
```
+0x00  self ptr / link
+0x04  parent ptr
+0x08  update fn ptr (entity walker: blx [ent+8])
+0x0c  halfword flags
+0x10  pos X (8:8 fixed)
+0x14  pos Y (8:8 fixed)
+0x18  pos Z (8:8 fixed)
+0x1c  target X (movement iterator)
+0x20  target Y
+0x24  target Z
+0x28  input velocity X (signed halfword, set by 020694b0)
+0x2a  input velocity Y
+0x2c  input velocity Z
+0x2e  applied velocity X (physics writes)
+0x30  applied velocity Y
+0x34  extra velocity (impact/spring)
+0x36  raw input X (per-frame scratch, zeroed at end)
+0x38  raw input Y
+0x3a  raw input Z
+0x3c  slope-accel source X (read by 020694b0, indexed into slope table)
+0x3e  slope-accel source Y
+0x40  gravity value (added to +0x30 each frame by 020694b0 @0x20695a0)
+0x42  terminal/fall velocity clamp (caps +0x30 @0x20695c4)
+0x48  accel clamp value (spring/transport)
+0x4a  AABB/hitbox offset X (with 0x4c, passed to 02068e58)
+0x4c  AABB/hitbox offset Y
+0x4e  saved velocity X (history)
+0x50  saved velocity Y
+0x52  saved velocity Z
+0x54  control flags: bit0=facing?, bit1(2), bit2(4)=busy, bit3(8), bit4(0x10),
                       bit5(0x20)=pausable, bit7(0x80)
+0x58  state flags: bit0(1)=grounded, bit2(4)=impact, bit4(0x10), bit5(0x20),
                      bit8(0x100)=spawning, bit11(0x800), bit12(0x1000),
                      bit13(0x2000)=frozen, bit0x4000=solid guard, 0x100000/0x200000 dir,
                      bit0x400000, bit26(0x4000000)
+0x5c  halfword status (bit4(0x10), bit6(0x40))
+0x5e  previous angle (halfword)
+0x60  prev angle stored by iterator
+0x62  player action timer (halfword): low nibble = action phase timer
       (decremented 1/frame at 0x206c4d8), high nibble (0xf0) = velocity-drift
       table index (decremented 0x10/frame at 0x206c4e8). When low nibble hits 0,
       +0x68 action fn ptr is called (gated: +0x54 bit7(0x80) and busy). Timer is
       set by actions via `strh rN, [r5, 0x62]` (e.g. intro spawner 0204d854 sets
       0x88 = index 8, count 8). High nibble (>>5, i.e. /8) indexes signed-byte
       drift table 0x2077944 (+0x6/+0x7 pos bytes += table[n] and table[n+1]).
       Table 0x2077944 = [1,1,-1,-1,2,2,-2,-2,4,4,-4,-4,-4,4,4,-4,2,2,-2,-2,0..]
       (0xffff0101 header word then signed bytes).
+0x64  collision box margin L (signed byte)
+0x65  margin R
+0x66  margin T
+0x67  margin B
+0x68  action fn ptr (player spawn sets fcn.0204d854 = intro/skip action; called
       by 0206c3d0 @0x206c528 when +0x62 low nibble == 0 and +0x54 bit7 clear)
+0x6c  action fn ptr 2 (called @0x206c4b4 when busy bit set in 0206c3d0)
+0x70  action fn ptr 3 (called @0x206c5f4 after timer/control checks)
+0x78  misc fn ptr
+0x7c  misc fn ptr
+0x80  object binding ptr (springs/platforms, read by 0206919c)
+0x84  partner/companion entity ptr (velocity injection, read by 0206919c)
+0x88  object ptr (partner ref; if partner+0x54&4 dead and not +0x54&0x200,
+       player marked dead @0x206c480)
+0x8c  action data ptr
+0x94  hitbox group A ptr (set by 02069f1c via 0203363c)
+0xa4  hitbox link fn (0206bd24: sets +0x94/+0xdc from +0xa4)
+0xc0/+0x108  flag words
+0xd8  halfword state
+0xdc  hitbox group B ptr
+0xec  ptr
+0x124 action state obj A (checked, freed via 0206aa04; written by spawner
       fcn.02069c74 @0x2069c7c; +0x138 obj link +0x150 param obj also set)
+0x128 action state obj B (written by fcn.02069934 @0x2069a70)
+0x12c action state obj C (written by fcn.02069934 @0x2069948)
+0x130 mover-controller binding obj (ctl+4 target / ctl+8 source; read by 0206919c)
```
- Player update flow (fcn.0206c3d0, 812B, verified): 
  1. +0x54&4 (dead) -> destroy entity 020371c8
  2. +0x54&8 -> OR bit4 (pending destroy)
  3. if +0x54&0x10 clear: AABB probe fcn.02068e58(pos +0x4a..0x52) -> hit: +0x54|=4
  4. +0x88 partner: if partner dead and not +0x54&0x200 -> +0x54|=4, clear +0x88
  5. fcn.0206919c (velocity injection) + 0206c798 busy gate
  6. +0x62 timer decrement (low nibble -1, high nibble -0x10)
  7. +0x68 action fn (if low nibble==0, not busy, +0x54 bit7 clear)
  8. +0x58&0x2000 gate -> fcn.020694b0 movement; 0x22c4ef0&8 && +0x58 bit8 clear
     -> clear +0x80/+0x84, fcn.0206bd5c movement iterator
  9. +0x62 high nibble -> drift table 0x2077944 applies to +0x6/+0x7
  10. +0x70 action fn; then if +0x124/0x128/0x12c action-state objs present ->
      fcn.0206c708 (free action-state objs via 0206aa04, then hitbox link 0206bd24)
      else hitbox link direct (0206bd24 with +0x94/+0xdc)
  11. +0x62 low nibble==0 and 0x22c4ef0&0x10 -> clear +0x36/0x38/0x3a, +0x6/+0x7
  12. +0x7c fn ptr call; exit
- fcn.0206c708 (124B, action dispatch): frees action-state objs +0x124/0x128/0x12c
  (each via 0206aa04 = passes [obj+1]/[obj+0x10]/[obj+0x14]/[obj+0x18] to action
  processor fcn.0206a0ac), then calls +0xa4 hitbox link (0206bd24) with +0x94/+0xdc.
- fcn.0206c798 (36B, busy gate): returns 1 if [0x22c4ef0]&1 (pause/freeze) AND
  arg (player +0x54) bit5(0x20) clear, else 0. Guards action/timer processing.

## Entity List (0x22b4574) + Entity Containers
- Global head at 0x22b4574. Container struct (0x1c bytes):
```
+0x00  prev link (or list node)
+0x04  next link (walker: r6 = [r6,4] until == terminator arg2)
+0x08  update fn ptr (walker: blx [ent+8])
+0x0c  destroy fn ptr
+0x10  DATA ptr (the actual entity struct, e.g. 0x134 player / 0x77c pause ctl)
+0x14  halfword flags (bit0 = skip in walk)
+0x16  halfword type (low 3 bits = phase/priority, bit4 = always-update)
+0x18  halfword size/cap
+0x1a  halfword marker (0xffff)
```
- fcn.02036e9c (68B): slot allocator - count at 0x22b4578 (cap 0x100), free-table at
  0x22b458c; returns next free container ptr, increments count; returns 0 if full
- fcn.02037480 (424B): list init (called by fcn.02030598 @0x203066c) - clears
  containers at 0x22b498c (stride 0x1c), fills table 0x22b458c with ptrs, links
  four special containers at 0x22b457c/0x22b4580/0x22b4584/0x22b4588 (types
  0x20/0xefff/0xf000), sets head 0x22b4574 = first special container
- fcn.02036e04 (144B): per-frame walker - head->[head], walk [ent+4] chain;
  skip if [ent+0x14]&1, no fn, or (global 0x2087cb8 bit31 && type bit4==0 &&
  type&7 < [0x2087cb8]&7); else blx [ent+8]. arg1 = entity to start at,
  arg2 = terminator
- fcn.020372d0 (396B): entity allocator - calls 02036e9c, sets [ent+8]=update cb,
  [ent+0xc]=destroy cb, [ent+0x16]=type, [ent+0x1a]=size; if size arg != 0,
  allocates data block (via 02034a40, magic 0xcccccccc/0x2467531) and stores at
  [ent+0x10]. Callers include the 0x134 player spawn (0204da44) and the 0x77c
  pause controller spawn (02055918)
- Global 0x2087cb8: system flag word (bit8 checked by state-change 02043c7c &
  teardown 02055878; bit31 = freeze entity updates)

## Player vs Pause-Controller Entities (RESOLVED)
- fcn.0204da44 (348B): spawns the ACTUAL PHYSICS PLAYER - 0x134-byte entity via
  020372d0 (update cb 0206c3d0, destroy cb 0206bef4), cleared via 020082e8,
  loads gameover action via 02069f1c (action data root 0x22c45f0), action init
  0206a054, teardown 02069818, hitbox group 02069918, facing 020698f0; sets
  +0x58=0x100|0x2000, +0x5c|=0x1|0x100|0x80, +0x54|=0x2|0x10,
  +0x68=fcn.0204d854 (intro skip action), pos from args. Called by
  fcn.0204525c (character-select/mode init) @0x204e004, @0x204e3dc.
- fcn.02055918 (1256B): spawns the 0x77c PAUSE/FIX OVERLAY CONTROLLER via
  020372d0 (update cb 0x20556a4, destroy cb 0x2055878, type 7). Cleared via
  020082e8. Sets up 5 hitbox/animation groups via 02069d38 (ac_fix_pause /
  ac_fix_pause_frame .bac, global 0x22c448c), camera slots +0x140..0x14c
  (0x6000/0x5800/0x4000), hitbox entry structs +0x4c.. (stride 0x48, flags
  0x20/0x30, +0x64=0xf/1), tile buffers +0xe4/+0x12c (0x200B each via 02008300),
  HUD sprites 3-9 (fcn.0201bf40), action IDs +0x164..0x170 (0x10/0x12/0x13/0x14
  or 0/0x37/0x38/0x39 per char mode). Called by aav.0x0204b95c @0x204ba48
  (gated on input bit 0x8 and zone-state checks).
- aav.0x20556a4 (update cb): pause/menu frame - reads 0x22b4574 head,
  [head+0x10] = physics player; copies input 0x22503e0 +0/+2/+4 into player
  +0x174/+0x176/+0x178; pause-state machine (menu idx [r5], clamp [r5,2]);
  dispatches 0204b7ec (HUD) / 02043c7c (state change) based on 0x22c4560+0x10
  (==2 -> mode 4, else mode 5), re-registers cb 0x205557c via [ent]+8;
  calls 020552bc (frame render), 020551b0 (tile render), 020371c8 (remove).
- CONCLUSION: the 0x134 struct IS the gameplay physics player (all velocity/
  gravity/hitbox fields). The 0x77c struct is a PAUSE/MENU overlay controller
  entity that wraps the player, NOT a second physics player. It is spawned on
  stage entry (aav.0x0204b95c) to provide pause/fix animations and HUD.

## Physics Constants (player)
- Speed/accel scale: `aav.0x0207cf7c` = 256 (0x100); accel = input * 256 >> 8 = input
- Velocity fields: +0x2e/+0x30 (X/Y), +0x34 (extra)
- Movement iterator step: 0x800 (halved to 0x400 airborne, flag 0x1 clear)
- Collision box margins: +0x64..0x67 (signed bytes)
- Direction encoding: `(facing+0x80 & 0xff + 0x20) & 0xc0 >> 6` -> 0-3 (R/D/U/L)
- Gravity: entity +0x40, applied as `+0x30 += +0x40` (020694b0 @0x206959c: smulbb x256)
- Terminal velocity: +0x42, `+0x30 = min(+0x30, +0x42)` @0x20695c4
- Ground probe distance: 0x18 (24 px), capped @0x2064f24 / 0x2064f2c
- Slope/step reject: gravity re-zero when grounded via +0x58 bit1 (@0x20652d0 chain)
- Ground snap threshold: mvn 0xd = -14 (@0x2064f70); collision = -0xffff..-14 block
- The four gravity/slope fields +0x3c/+0x3e/+0x40/+0x42 are written by action-data
  load (fcn.0203363c stores arg_24h/arg_28h there), i.e. per-action physics params

### fcn.020648c4 (3416B) - ground collision + slope snap
- Reads +0x80 (misc state), +0x10/+0x14 pos, byte+1/+5 facing
- 4-case direction switch (R/D/U/L) selects probe offsets from +0x64..0x67 margins
- Ground probe via fcn.02066010 (tiles/entities) + fcn.02067670 (entity sweep):
  - two corner probes, takes min (`movge`); sets +0x58 bit0x100000 when clear
  - mid-slope probes loop `r4` from 1..step (slope-follow, keeps angle var_64h)
- Slope fall detection: if +0x60 low byte & 8 && +0x58&0x10 && dir vertical ->
  probe ahead; if gap -> set r8 = 0x18 (fall through)
- Ground contact: if probe r8 <= 0, set +0x58 bit1(2) (@0x206556c); if r8 >= -14
  push out by r8 along facing dir (4-case switch @0x2065580), then clear +0x2e/+0x30
  (velocity kill on landing unless bit0x4000)
- Grounded state: +0x58 bit0(1) = on ground; bit1(2) = contact this frame
- Position adjust: `+0x10 -= r7`, `+0x14 -= r6` (movement delta applied)

### fcn.020657dc (1740B) - movement + collision resolution (see Collision section)

## Slope Table (0x206e470)
- Literal `ldr r3,[0x2065620]` = 0x206e470 (also used by fcn.020693f0 rotation)
- Paired halfwords (value, 0x1000 mask); indexed at facing*32 halfwords
- Facing 0..31 -> 0, 101, 201, ..., 2824 (quarter-ramp; slope data, not pure sine)
- Referenced by fcn.02032480 (ldrsh [r2,r1]) and fcn.020693f0 (slope angle rotation)

## Object-List System (fully mapped)
Two global lists, bases at 0x208922c (primary) and 0x2087d04 (secondary),
selected by table 0x2072c78[arg] (arg 0/1). Managed per-frame by fcn.02036a0c.

### Layout (relative to base)
```
base + 0x800 : control header
  +0x9c : object count (capacity 0x80)
  +0x9e : camera-space A count
  +0x8a0: camera-space B count (sum must be < 0x20)
base + 0x8a8 + i*0x20 : 32-byte camera-space entries
  4 signed halfword coords at +6/+0xe/+0x16/+0x1e
base + 0x1000 + i*8 : 8-byte object entries
  +0xae : next index in chain (0xffff = end)
base + 0x1400 + slot*2 + 0xa8 : per-slot list heads (32 slots)
base + 0x14a8 : 0x40-byte buffer (filled 0xffff on reset)
base + 0x14e8 : 0x40-byte buffer (filled 0xffff on reset)
```

### Functions
- fcn.02036a0c (200B): per-frame walker; iterates 32 slots, walks each +0x1000/+0xae chain,
  copies 6 bytes/object into compact table at base+0x8a8 (via fcn.02008300); resets
  buffers at +0x89c/+0x89e and the two 0x40-byte areas
- fcn.02036bfc: insert object; args = base-index, slot-index(0-0x1f); appends to
  per-slot chain, increments +0x9c; checks 0x80 capacity
- fcn.02036aec (260B): camera-space lookup; arg2 = 4 word coords, shifted <<0xc,
  searches 32-byte entries for matching halfword coords; returns 0xffff if full.
  Callers: fcn.02032480 (level/camera space), fcn.0206aa38
- fcn.02036bfc: insert object; args = base-index, slot-index(0-0x1f); appends to
  per-slot chain, increments +0x9c; checks 0x80 capacity

## Tile/Object Placement Chain (0206aa38 / 0206b244 / 0206b768)
- fcn.0206aa38 (2032B): stage tile-object placement builder. Args: r0=ctl struct
  (sl), r1=tile-data base, r2/r3 = X/Y range (abs must be < 0x22 = 34 tiles).
  - reads [sl+0x10] = tile entry data ptr, [sl+0x24] = entry table, [sl] = list
    index into aav.0x02072c78 (base 0x208922c)
  - camera-slot budget: [base+0x800+0xa0] + [base+0x800+0x9e] + 4 must be < 0x20
    (else bails via fcn.02032d2c)
  - builds 4x4 transform at sp+0x70 (matrix via fcn.02000cb4 init / fcn.02000d98
    mult), scales pos [sl+4]/[sl+6] by aav.0x0207cf84 table (0x3ff masks)
  - main loop (0x206adc4): iterates tile entries [sb] (stride 8), extracts 2-bit
    direction (0xc000 masks -> 0-3), for each: camera-slot lookup via fcn.02036aec
    (writes 4 halfword coords into base+0x8a8 + slot*0x20: +6/+0xe/+0x16/+0x1e,
    coords >>4), then insert object via fcn.02036bfc with [sl+0x46] slot base;
    writes entry halfwords: +0 = 0x4100|flags^[arg+0x14], +2 = pos&0x3ff with
    dir bits (0x3e00 bic/orr), +4 = tile | counter | flags
  - flag word [sl+0x2c]: 0x2000 adds camera offset +0xa2/+0xa4; 0x80->arg18|0x1000;
    0x100->arg18|0x2000; 0x1000->arg14|0x1000; 0x200->arg14|0x200;
    0x4000 enables bounds clamp (r4 8..0xc0, r5 0x100)
  - constants: 0x3ff mask @0x206b230, 0x1ff @0x206b240, dir table 0x2072da0
    (0x80008-based, per-dir halfwords), 0x2072c70 base
- fcn.0206b244 (1296B): sibling builder (same structure, reads [sl+0x10]/[sl+0x24],
  scale [r5+0x94], same flag handling +0x2c). Used for the secondary tile set.
- fcn.0206b768 (1444B): tile placement dispatcher. Reads entity hitbox link args;
  sets [r8+0x2c] flag word (0x4000, bic 0x184, bits 0x4/0x80/0x100 from [sb+0x5c]),
  camera offset 0x236aad8 (+4/+8 >>8), mode@0x22c4ef0 bit2, writes pos to
  [r8+4]/[r8+6]. Dispatches per tile state: both sides 0x1000 -> fcn.0206b244,
  else fcn.0206aa38; r7/r8 empty cases -> fcn.02032d2c (remove) or fcn.02032480
  (lookup). Called by fcn.0206bd24.
- fcn.0206bd24 (56B): hitbox link fn (entity +0xa4) - passes [r3+0x10]/[r3+0x14]
  (entity obj data ptrs) to fcn.0206b768. Called by player cb fcn.0206c3d0 @0x206c658
  and action dispatch fcn.0206c708 @0x206c770.
- fcn.0206bd5c (408B): movement iterator (player chain, called by 0206c3d0 @0x206c56c)
  - steps [sl+0x10]/[sl+0x14] toward target [sl+0x1c]/[sl+0x20] by 0x800 (halved to
    0x400 when +0x58 bit0x100 (airborne)); flag +0x58 0x200000 gates the whole step;
    stores prev angle +0x60 = +0x5e

## ARMIPS Extraction Framework
- `scripts/extract.py <ram_addr> [size]`: reads ARM9 bytes from ROM (file = 0x4000 + (ram - 0x02000000)),
  writes `disassembly/<addr>.bin` + an ARMIPS harness `<addr>.asm` (.create at 0x02000000, .org, .incbin),
  then reassembles with `tools/armips` and verifies byte-exactness against the ROM (exit 0 = match).
- `tools/armips`: built from source (Kingcom/armips @ v0.11.0, cmake+ninja, ARMIPS_USE_STD_FILESYSTEM=ON).
- 12 documented functions extracted and verified (main loop, input, object-list, player update,
  velocity injection, movement handler, collision, ground/slope, spawn).
- To patch: edit the .bin or replace .incbin with hand-written ARM instructions, re-run armips,
  then apply the .bin bytes back into a ROM copy at the mem->file offset.

## Action / Object-Name System
Objects and actions are named via `obj:<name>` strings resolved to `.bac` animation
data (Sega Bone Animation Controller). Full chain mapped.

### Name resolution (fcn.020687b8, 412B - object-name cache resolver)
- Builds `"obj:" + name` via strcat fcn.0202c474, resolves via hash lookup
  fcn.0202128c, caches result at [r5] with a +0x8000-valid-bit generation counter
  at [r5+4]. 13 callers, all in the action-object region (02068494, 02068600,
  020697/698/699/69a/69c/69d/69f...).
- Hash/string lib: 0200be70 (init), 0200bb20 (search), 0200bad8 (parse),
  0202c474 (strcat), 0202c5f0 (strlen). `obj:` literal at 0x207cf68.
- `obj:` namespace = .bac animation files. `/act/` path strings (364 occurrences)
  e.g. 0x2078b14 `/act/navi_b.bac`, 0x2079240 `/act/dm_cldm_tatk.bac`,
  0x207a1bc `ac_fix_gameover`, 0x207ba34+ `ac_fix_pause`/`ac_fix_pause_frame`,
  0x207c5d4 `poly_demo_under2`, 0x2126aa0 `son.bac`, 0x21272c0
  `ac_eff_entrance`/`ac_ene_gun_hunter`, 0x2073b38+ `act/dm_demoplay.*`,
  each with `_jpn/_eng/_fra/_deu/_ita/_spa` locale variants.
- BAC container chunk tags (fcn.020257f8 parser): BVA0(0x30415642)/BMA0(0x30414d42)/
  BCA0(0x30414342)/BTA0(0x30415442)/BTP0(0x30505442) at 0x2025a34..0x2025a44.

### Object spawner framework (0x2069900-0x2069f00)
- fcn.02069934 (412B, verified): spawns action objects - resolves `obj:` name ->
  caches at [sb+0xf4], spawns via 02031878, sets type bytes +0xa=0x13/+0xb=6,
  +0xfc|=1, +0xf8=stack-arg, stores action at [sl+0x12c] (action-state obj C) and
  [sl+0x128] (action-state obj B) via @0x2069a70; +0xf8 action data
- fcn.02069a34 (256B, verified): links child objects via +0x128/+0xac chain,
  sets +0xa8|=1, clears/retargets +0x128 link
- fcn.02069b34 (320B, verified): parses name string (0202c5f0 length), spawns
  sub-entity via 020319a4/020319f0/02031c1c, sets +0x114
- fcn.02069c74 (500B, verified): action-state spawner for obj A - writes obj to
  player +0x124 (@0x2069c7c), sets +0x168|=1, +0x138 = resolved action obj,
  +0x150 = param obj, +0x114; resolves obj:<name> via 020687b8, spawns via
  02031c1c/020319f0. Name fragments at 0x2069c60: "at"/"ac"/"am"/"pt"/"av".
- fcn.02069d38 (292B): set-hitbox-frame-from-named-object - resolves obj:<name>,
  reads frame slot, allocates N consecutive camera slots via fcn.02039c18.
  Callers: fcn.02055918 @0x2055b44/0x2055b6c/0x2055b94/0x2055bbc
- fcn.02069f1c (300B): load-action helper - stores action data at [sb+0x8c],
  resolves obj:<name>, allocates 2 hitbox slot groups via 02039c18, sets hitbox
  frames +0x94/+0xdc via 0203363c with flags 0x5000200/0x5000600.
  Called by player spawn fcn.0204da44 @0x204dad0 (with action-data root 0x22c45f0)
- fcn.02039bb0/02039c18: object-slot allocation - finds N consecutive free slots
  in the camera object list (aav.0x02072c78 arg-indexed table; base 0x208922c;
  slot struct with +0x94/+0x98 halfwords). 02039bb0 = free slot (used by teardown)
- Helpers: 02034a40 (alloc), 020380d8/020380a0, 02031a68 (free obj),
  02033550 (release slot chain). Sega obj_act config magics 0xcccccccc/0x2467531
  at 0x2068590/0x2068594.
- fcn.02069818 (164B): action teardown - clears +0x5c bits, frees +0x124 via
  02031a68, +0x12c/+0xa4 via 02033550
- fcn.02069918: hitbox-group setter trampoline (bx 0x206990c, passes +0x94/+0xdc);
  0206990c (12B) stores a halfword tag to [+0x94+0x46] and [+0xdc+0x46]
- Action-state slot mapping: +0x124 = spawner 02069c74 (action-state obj A,
  freed by 0206c708@0x206c720), +0x128 = 02069934@0x2069a70 (B, freed@0x206c734),
  +0x12c = 02069934@0x2069948 (C, freed@0x206c748). All freed via 0206aa04 which
  hands [obj+1]/[obj+0x10]/[obj+0x14]/[obj+0x18] to action processor 0206a0ac.

### Action state machine
- fcn.0206cccc: action-ID registry - reads usage counts (0x22c4f14 count byte,
  0x22c4f18 halfword table, 0x22c4f1a counters), stores ID at +0xd8/+0x120
- fcn.0206a054 (88B): action init - reads [r4+0x8c] action data, calls 0206cccc,
  ORs 0x10 into +0xc0/+0x108 flags
- fcn.0206aa04 (52B): action-state free - reads state obj +0x10/+0x14/+0x18,
  calls fcn.0206a0ac. Called by fcn.0206c708 @0x206c720/0x206c734/0x206c748
- fcn.0206c708 (124B): action dispatch - frees +0x124 via 0206aa04, +0x128, +0x12c
- fcn.0206a0ac (2344B): action-state processor - reads physics constants
  aav.0x0207cf78=0x100, aav.0x0207cf7c=0x100, aav.0x0207cf80=0x1000100,
  aav.0x2084d40=0/0x2084d38=0, slope table 0x206e470. Called by fcn.0206aa04.
- fcn.0206c7f0 (312B): event/script dispatcher - reads [sl] flags, [sl+4] halfword
  table ptr, [sl+8] cb; loops over halfwords calling fcn.02021ad8 per entry
- fcn.0206c798: pause check - returns 1 if mode@0x22c4ef0 bit1 set unless ctrl+0x20
- 0x22c4ef0 mode word (bit0 = pause): 9 code refs incl 0x206c7bc (clear bit0),
  0x206c7d8 (set bit0), 0x206c7ec (mode-> 0206c7f0 dispatcher)

## Stage Player (pause/menu entity, struct size 0x77c)
- fcn.02055918 (1256B): stage-entry player spawner. Registered via pointer table
  (no direct callers); allocates a 0x77c-byte entity via fcn.020372d0 (generic
  alloc, 396B, 40 refs) with update cb 0x20556a4 and destroy cb 0x2055878.
  Sets hitbox/animation groups via 02069d38 (global roots 0x22c448c/0x22c4560+0x18/
  0x22c4590), slot chains at +0xb0/+0xf8 via 02033550, constants +0x164..0x170 =
  0x10/0x12/0x13/0x14, 6-case switch (table @0x2055c98, idx r6 0-5 = character
  modes). Calls 0204b7ec (HUD font/id setter) and ends with 0204b7ec.
- aav.0x020556a4 (140B): per-frame update (entity walker blx [ent+8]):
  - reads entity-list head 0x22b4574 -> [r0,0x10] = player object
  - copies input (0x22503e0 +0/+2/+4) into player +0x174/+0x176/+0x178
  - pause-state machine: menu index [r5] clamped to [r5,2], +0x164..0x170 =
    action IDs (resume/quit), on input bits 0xc0/0x80 dispatches 0204b7ec (HUD)
    and 02043c7c (state change) based on zone-state 0x22c4560+0x10 (==2 -> mode 4
    else mode 5), re-registers cb 0x205557c via entity [0] +8, calls 020552bc
    (frame render) + 020551b0 (tile/compression render, nibble table 0x207ba24)
  - This is a PAUSE/MENU controller, NOT the physics player. The physics player
    is the 0x134 struct from fcn.0204da44. Relationship to be confirmed.
- aav.0x02055878 (140B): teardown/reset - frees 4 hitbox groups via 02039bb0
  (loop r6 0..3, group stride 0x48, [r7+i*0x48+0x54]), reads list 0x2087cb8
- fcn.02043c7c (276B): state-change fn - reads 0x2087cb8 flag (bit8 -> r2=7),
  writes 0x22c4410 struct +0x16 halfword, spawns object via 020372d0 (size 0x48?)
- fcn.020372d0 (396B): generic entity allocator (arg0 = update cb, arg1 = type)

## Stage/Zone State Structs (RAM globals)
- 0x22c4560 (zone-state, 127 refs) / 0x22c4590 (zone-substate, 16 refs) - same
  struct layout (shared tail +0x58/+0x5a/+0x5c). Offsets seen in code:
  +0x0  base/self ptr; +0x2/+0x6/+0xa halfword flags; +0x4/+0x8/+0xc/+0xe words;
  +0x10 mode word (020556a4 compares ==2); +0x14/+0x15 state byte+flags;
  +0x18 flag word (bit0 = active/advance); +0x1c/+0x1d/+0x1e flag word+bytes
  (0x1c/0x1d heavily accessed: 97/89 hits); +0x20/+0x24 counters;
  +0x28/+0x34/+0x48/+0x4c/+0x50/+0x58/+0x5a/+0x5c words; +0x64/+0x70/+0x98/
  +0xc8/+0xcc/+0xce words (0xce = flags); +0xac write; +0x488/+0x48c (x8 each)
- 0x22c448c (object-cache root, 7 refs): +0x0 ptr. 0x22c45f0 (action-data root):
  only code ref = 0x204dbb0 (player spawn passes it as action data to 02069f1c)
- 0x22c4410 (stagemode, 4 refs): +0x0 word, +0x10 read; +0x16 halfword flags
- 0x236ac60 (world-object root, 4 refs): +0x0 self, +0x4/+0x8, +0x34, +0x50,
  +0x64, +0x98, +0xae (object idx), +0xc8/+0xd8, +0x218
- 0x22b4574: entity-list head (walked by 02036e04; [head+0x10] = current entity)
- 0x2087cb8: global flags word (bit8 checked by 02043c7c and 02055878)
- 0x22c4410: stage/mode-transition entity data ptr (owned by 02043c7c, update cb 0x2043b78)
- 0x22c4490: game-state context (0204b718: +0x4 mode ctx, +0x48 stageID, +0x4c/+0x4d
  stage bytes from zone-state; 0204b6cc looks up 0x20745a8 table -> 0204afc8 dispatch)

## Stage/Mode-Transition System (fcn.02043c7c family)
02043c7c(mode, data) = stage/mode transition INITIATOR. Allocates a 0x3e00-sized
"mode" entity (020372d0; update cb 0x2043b78, destroy 0x2043c68), saves data ptr to
global 0x22c4410, sets [data+2]=0x1100 (bit2 path) / 0, [data+4]=arg0(mode),
[data+6]=arg1(0x100). Callers pass ASCII stage codes: r0=0x43'C'/0x42'B' with
r1=0x100, or r0=5/r0=0x42 with 0x5000000. Reads 0x2087cb8 bit8 for variant path.
- aav.0x02043b78 (220B, update cb): FADE/COUNTER machine. Decrements [data+2]
  counter toward [data+4] target; ORs 0x20 into [data] when limit reached; removes
  entity via 020371c8 unless [data]&4. Called for 0x3e00 mode entity each frame.
- fcn.02043abc (172B, mode-setup): reads [data] flags (halfwords +0/+2), derives
  value (r3 = 0x10 if +2>=0x1000 else +2>>8), writes to 0x208922c+0x48 and
  0x2087d04+0x48 (object-list camera/level space); bit0x40 -> clears +0x40 flag and
  dispatches 0x400006c handler at 0x2043b40+.
- aav.0x02048ba4 (mode-data loader, update cb): reads stageID [data+4], indexes
  per-stage table 0x2073bd0 (stride 0x22) writing zone-state 0x22c4560:
  +0x1c/[+0x1d] = stage bytes (table +0x0/+0x1), +0x0 = table +0x2 half, +0x18 |=4,
  +0x20 = key checksum; on valid key ORs +0x18|=8, +0x24; calls 0204d6d0(3),
  sets mode word +0x10 = 3, loads stage via 02068094. Finalizes mode=3, then
  dispatches on [0x208922c]&3.
- Per-stage table 0x2073bd0 (stride 0x22): entry [+0x0]byte -> zone+0x1c,
  [+0x1]byte -> zone+0x1d, [+0x2]half -> zone+0x0 stageID, [+0x4]half (checksum /
  key04.bin presence), [+0x6]half, [+0xa]string "/dat/key/keyXX.bin" (04,01,02,03).
  Entries: stage[0]=01 02 0001 c0 01f2 key04; stage[1]=00 01 0000 4298 0d12 key01;
  stage[2]=00 02 0000 c0 01f2 key02; stage[3]=01 00 0001 0132 07f2 key03.
- aav.0x02048ad4 (mode-data per-frame cb): frees 4 action slots (+0x68/+0x6c/+0xe4/
  +0xe8) via 02039bb0; processes heap block (0x2467531/0xcccccccc magics) via
  0200831c/020067c0; calls 020349b8; clears zone +0x18 bits 2/4/8.

## Mode/Game-Flow State Machine (0x22c4560+0x10)
Mode word +0x10 (written only at 0x2048ccc(=3), 0x204c158(=0), 0x204d81c(=2))
selects the top-level game flow via a chain of entity[0].update installs:
- fcn.0204d6d0(mode): creates a mode entity (020372d0, update cb 0x204bf24) with
  [data+0]=mode; reads zone +0x18 bit0x40 (clears bit1 if set), +0x1c/+0x1d to pick
  fcn.0204d214 vs 0204ce7c setup. Called with mode=3 by stage loader.
- aav.0x0204bf24 (52B, update cb): calls 02033d60 (touch input enable/reset), then
  installs aav.0x0204bed0 as entity[0].update.
- aav.0x0204bed0 (76B, mode dispatcher): calls 0204b718 (game-state ctx write);
  branches on [data]&1 -> install aav.0x0204be9c else aav.0x0204be48.
- aav.0x0204be9c: calls 0204b700 (mode-table dispatch via 0x2073c84); if 1 ->
  install aav.0x0204be48.
- aav.0x0204be48: calls 0204b718; branches on [data]&2 -> aav.0x0204be14 else
  aav.0x0204bdf8.
- aav.0x0204be14: calls 0204b6cc (stage-table lookup 0x20745a8 -> 0204afc8); if 1 ->
  install aav.0x0204bdf8.
- aav.0x0204bdf8 (20B): installs aav.0x0204bd94 as entity[0].update (terminal
  gameplay handler).
- aav.0x0204b718(0): zeroes 0x60 bytes at 0x22c4490, then writes +0x4=arg,
  +0x48=stageID, +0x4c/+0x4d = stage bytes from zone-state.
- fcn.0204bf88 (1428B, in-game setup): creates HUD entity (020372d0, 0x10 bytes,
  cb 0x204ba84) + pause-overlay entity (cb 0x204b95c); loads stage music via
  02022270 from table 0x2074960 indexed by +0x1c*0x12/+0x1d*6; sets camera/slot
  structs 0x22c4efc/0x22c4ee8 (+0x12=0xfff, +0x14/+0x16=0, base=0xb50); calls
  0206ce80 (object system init), 02004084.
- aav.0x0205aae4 (act/gameplay setup entity): [data+4]=player data ptr, [data+8]=
  action ptr (written to player +0x70), +0x480/+0x484 = player spawn pos, +0x488/
  +0x48c = per-act flags; sets player +0x5c|=0x40, clears +0x32c bits, zone +0x18|=1.
- Action slot writes on physics player: +0x68 at 0x204db90 (spawner 0204da44 sets
  fcn.0204d854 intro-skip), +0x70 at 0x205ab44 (act setup) / 0x205b4fc.

## HUD / Pause Overlay callbacks (0x204ba84 / 0x204b95c)
- Both are entity update cbs registered by fcn.0204bf88 (in-game setup) via
  fcn.020372d0: entity 1 = cb 0x204ba84 (r1=0, r2=0x10, r3=0x10); entity 2 =
  cb 0x204b95c (r1=0x204b958=bx-lr stub, r2=0, r3=0x4000).
- aav.0x0204ba84 (44B, verified): short chain - calls fcn.0201d058(0),
  fcn.0201d010, fcn.0201cf04, fcn.020204c0, then bl 0x22fabbc (ARM7/loaded code).
  The 0201dxxx chain all gate on aav.0x02083e10 (a struct ptr; functions no-op
  if [0x2083e10]==0, or if [+4]!=expected state 1/2). fcn.0201d058 also checks
  [+4]==0 and then works on +0x3c. fcn.020204c0 checks aav.0x020779ec==1 else
  calls 0201d010. This is the HUD/IRQ sync stub.
- aav.0x0204b95c (272B, verified): per-frame HUD/score/camera sync:
  * reads [0x22c4590+0x14] (zone2 state byte) -> bl 0x23375f8 (loaded code)
  * reads camera [[0x236ac60]+0x300+0xae] halfword -> bl 0x2337634
  * [0x22c4560+0x20] -> bl 0x2337618; if zone +0x18&1, increments +0x20
  * increments zone +0x24; camera halfword [0x236ac60+0x300+0x50]<<0xc>>0x10
    -> bl 0x2336050
  * if [0x22c4ef0]&0x80 -> skip score block; else score getters 02044690
    (struct@0x22c4478, [+0x10] signed byte+6) + 02044670 (byte+7) -> fcn.0206c784
    stores 2 bytes at 0x22c4eec; then negated values written to
    [0x236aad8+0x18]/[+0x1a] and copied to +0x7c/+0x7e
  * calls fcn.0206ded8 (reads 0x22c5664/0x22c5668), fcn.02067cc8 (0x22c4de0)
  * input [0x22503e0+4]&8 -> fcn.02055918(0) (set-hitbox-frame)
  * zone +0x18&0x20 -> fcn.0206d044 (text/cheat input)
- aav.0x0204dbb8 (252B, intro countdown/timer cb, called from 0x204e0a4/0x204e47c):
  reads player entity [0x22b4574]->[0x10] (+0x10=player), countdown halfword [+2]:
  * >= 0x1f4 (500): if no skip keys (mask 0xc0b=A|B|L|R|dpad) AND touch-state
    0x22b65b4+0x10 press-edge bit6 clear -> reset to 0x1f4
  * == 0x168 (360) -> fcn.020446b0(2); == 0x170 (368) -> fcn.020446b0(4)
  * == 0x200 (512) -> fcn.02043c7c(5, 0x100) (mode transition)
  * > 0x200: fcn.02043a58 (fade); if nonzero -> 02037134 (remove entity) +
    02054e4c, exit
  * else score getters 02044690/02044670 -> fcn.0206c784 (bytes to 0x22c4eec)

## Terminal Gameplay Loop (0204bd94 / 0204b528)
- aav.0x0204bd94 (68B, terminal per-frame handler): calls 0204b4cc + 0204b470; reads
  mode word +0x10; if 0 and zone +0x18&0x40 -> 0204c9b0 + 0204c8e8, else 0204cc48;
  then installs aav.0x0204bd6c.
- aav.0x0204bd6c (40B): removes entity[0] (020371c8) and calls 0204bf88 (in-game
  setup) — one-shot wrapper around the setup function.
- fcn.0204b4cc (84B): builds an object filename string from 0x22c448c + "com" + "-"
  + "." (string concat lib 02021310/02021264/020257f8) -> 020212e0 (object loader
  consumer). Called each gameplay frame; also from 0204bbe4.
- fcn.0204b470 (80B): STAGE UPDATE DISPATCHER. Indexes per-stage handler table
  0x20741f0 by zone +0x1c*0xc + +0x1d, then blx handler. Also calls 02036820 (from
  0x22c4484), 0204c748, 0x233bc40/0x2335954. Same table used by 0204b528.
- fcn.0204b528 (228B, per-frame stage update): dispatches second per-stage table
  0x2074184 by +0x1c*0xc + +0x1d (120 entries, stride 0xc, 10 zone-rows x 12 act
  slots; unique handlers 0x2049254..0x204afc0); frees cached object block at
  0x22c4488 (heap 0x2467531/0xcccccccc via 0200831c/020067c0, then 020349b8,
  clears 0x22c4488); calls 0204b2ec + 020451c8 (late-stage handler, +0x1c>=9 or
  +0x1d>=2). Index math: r0 = table + zone*0xc, then ldr [r0, act*4].
- Table 0x2074184 layout (verified):
  * Rows 0-5 = main zones (zone index = +0x1c). Handlers are mostly tiny stubs:
    `bx lr` no-ops (0x2049254-0x2049268, 0x204a234, 0x204a230, etc.) and small
    object-loaders (~40-100B) that read 0x22c4488 / write 0x2369ad0-0x2369ba0.
    Row 0 (zone 0): 0x204a234 bx-lr (act0/1), 0x204a184 loader, 0x204a0d0 loader,
    0x204a018/0x204a014, 0x2049f68...
  * Rows 6-9 = special/boss zones: slots hold file-string ptrs (0x2079734,
    0x2079810, 0x20792ac, ...) that are NARC/mod resource paths, plus repeated
    sentinel handlers: aav.0x204afb8 (`mvn r0,0` = returns -1) x8 and
    aav.0x204afc0 (`mov r0,0` = returns 0) x2, plus real loaders 0x204a98c,
    0x204aee8, 0x204aecc. Zero slots for unused.
  * Resource strings (0x20792ac..0x2079fa8): /narc/z13_map.na, /narc/z53_map.na,
    /narc/z13_eve.na, /narc/z53_eve.na, /narc/z13_act_lz.., /mod/bs1_base.nsbmd,
    /mod/bs1_neck.nsbmd, /mod/bs1_hammer.nsbmd, /mod/bs1_sky.nsbmd,
    /mod/bs1_sky.nsb, /mod/bs1_neck.ns, /mod/bs1_base.ns, /mod/bs1_hammer. (nsbmd
    model files; z13/z53 = zones 1/3 and 5/3 maps/events).
  * aav.0x2074308 = resource-file table (stride 0x10, 6+ entries): entry = {string
    ptr, sentinel handler (0x204afb8=-1 / 0x204afc0=0), 0, loader fn}. Entry 0-3:
    bs1_base/neck/hammer/sky.nsbmd -> 0x204a98c; entry 4: z53_map.narc ->
    0x204aee8; entry 5: z53_eve.narc -> 0x204aecc.
- aav.0x204a98c (244B, model/resource loader): iterates table 0x2074308 (4 entries,
  stride 0x10) comparing string ptrs via fcn.0202c348 (string match on arg2); on
  match loads [r5+0x5c] via 020257f8 + 020380d8, allocates 0x20-aligned block via
  02034a40 (0x208fe80 heap), stores result at 0x2369ad0[r4]; if r4==3 additionally
  calls 0205453c(1, [0x22c4560]) + 0204525c.
- aav.0x2049c40 (72B, main-zone object loader): reads [0x22c4488] into [0x2369ba0],
  builds "exc"/"com" filename from aav.0x207a08c via 02021310, calls 02021264 +
  020257f8 + 020212e0. NULL-ptr early-outs. Other row-0/1 loaders (0x204a184,
  0x204a0d0) follow same pattern writing 0x2369ae8/0x2369ad0.
- aav.0x0204a238 = object-name table matcher (iterates ptr table 0x2074498,
  compares via 0202c348).
- fcn.0204b2ec (36B): calls 02036894, clears 0x22c4484.
- Stage object-loaders (0x2049c40..0x204a234, both dispatch tables): each reads ptr
  from 0x22c4488 (stage-object data), builds "exc"/"com" string, calls 020212e0
  loader; NULL-ptr early-outs. aav.0x0204a238 = object-name table matcher (iterates
  ptr table 0x2074498, compares via 0202c348).
- Global pointers: 0x22c4488 = current stage-object data ptr (cleared each frame),
  0x22c448c = object-cache root (read by 0204b624/0204b4cc), 0x22c4484 = cleared by
  0204b470/0204b2ec.

Gameplay flow so far: stage select -> 02043c7c(mode,data) creates mode entity ->
02043abc writes level/mode -> 02048ba4 loads per-stage data + keyXX.bin -> mode
word=3, 0204d6d0(3) -> 0204bf24 -> 0204bed0/0204be48/0204be14 -> 0204bd94 terminal
gameplay (calls stage dispatcher 0204b470 + per-frame stage update 0204b528), with
0204bf88 building HUD + player + music + camera.

## Touchscreen State (0x22b65b4) & Hardware Controller (0x207f02c)
- `0x22b65b4` is a 0x28-byte touchscreen history/edge structure updated each frame
  from the main loop by `fcn.02033c60` (touch-state dispatcher). It is not a
  general game-state dispatcher. Confirmed layout:
  ```
  +0x00/+0x02  current X/Y (coordinate+1; 0xffff when idle)
  +0x04/+0x06  previous X/Y
  +0x08/+0x0a  press/accepted X/Y used by loaded title handlers
  +0x0c/+0x0e  release/older X/Y history
  +0x10        flags: bit0 enabled; bit1 selects alternate sample path;
               bit0x10 current contact; bit0x20 previous/held state;
               bit0x40 press edge; bit0x80 release edge; bit0x100 unresolved error
  +0x14        configured sample-history count/enable byte (observed 1)
  +0x16...     optional coordinate-history entries
  ```
  Runtime tap `(128,60)` gives `(129,61)` / `0x003d0081`; flags progress
  `0x01 -> 0x51 -> 0x31 -> 0xa1 -> 0x01` over idle/press/hold/release/idle.
- `0x22b658c` is an adjacent 0x28-byte sample table used by the alternate
  `0203387c` path with 8-byte entries. It was previously called a state table;
  its exact ownership and semantics remain unresolved.
- `fcn.02033c60` (72B): touch-state dispatcher. If flag bit0 (enabled) is clear,
  returns. Bit1 chooses `fcn.0203387c` (alternate table path); otherwise it
  calls `fcn.02033a8c` (main polling path).
- `fcn.0203387c` / `fcn.02033a8c`: touchscreen sample/history and
  edge-state handlers. Both preserve prior coordinates, update flags, write new
  X/Y or `0xffff`, and generate press (`0x40`) / release (`0x80`) edges.
  `0203387c` indexes the adjacent 8-byte sample table `0x22b658c` via the
  controller index (`fcn.02009544` -> `[0x207f02c+0xc]`); `02033a8c` polls the
  controller path directly and copies sample slots with `fcn.020091a4`.
- `fcn.02033cac` (140B): disables touchscreen updates; on alternate mode waits
  through `fcn.02009554`/`fcn.0200918c`, clears flags bit0/bit1, clears `+0x14`,
  and clears `0x2087cb8` bit15.
- `fcn.02033d40` (28B): clears enabled bit0 and `+0x14`. Called by `0204bc90`
  and a null-check handler.
- `fcn.02033d60` (128B): resets both 0x28-byte regions, sets `+0x14=1` and enabled
  bit0, and calls `fcn.02033cac` first when alternate-mode bit1 is set.
- `fcn.02033de8` (116B, init): calls input init `fcn.02009908`, zeroes both
  regions via `fcn.020042e8`, registers controller callback `0x02037c5c` via
  `fcn.020096f0`, then `fcn.0200985c` and `fcn.02009714`. Called from boot
  `0x202c69c`.
- `0x207f02c` is a hardware-adjacent task/screen controller. Known layout:
  `+0x0` callback, `+0x4..+0xa` four halfword params, `+0xc` sample/index,
  `+0x10` optional 8-byte-stride table pointer, `+0x14` count, `+0x18..+0x2c`
  DMA/coordinate params, `+0x30` flag, `+0x32` mode, `+0x34` wait flags, and
  `+0x36` wait mask.
- `fcn.02009544` returns `[0x207f02c+0xc]` (sample index/counter);
  `020096f0` sets the callback; `020091a4` copies one 8-byte sample slot from
  the controller history (checks the `+0x30` flag); `0200918c`/`02009178` poll
  the `+0x34`/`+0x36` wait flags; `020095f8`/`02009650` poll/capture the
  channel-6 controller state; `02009998` is the sampler that reads shared
  `0x027fffaa`/`0x027fffac` into the controller ring (`+0xc` index, `+0x10`
  table, `+0x14` limit).
- `fcn.0200d998` (empty `bx lr` stub) is registered as the channel-6 input
  callback by `fcn.02009908`; it is distinct from the touch family above.

## Next Steps
1. [DONE] +0x130 resolved as mover-controller binding (not dpad source); dpad = 020694b0
2. [DONE] Runtime cross-check via headless desmume-0.9.13 driver (hdrv): boot ROM,
   commit input each frame (NDS_setPad + begin/endProcessingInput), reach attract-
   mode demo, read live player struct. CONFIRMED: +0x28/+0x2a vel (velx accel
   +0x18/frame on ground; vely integrates +0x2a/frame during freefall = active
   +0x40 gravity 0x002a), +0x30 tracks vely in fall, +0x58 flags 0x00c1 grounded /
   0x80d0 airborne, +0x40 gravity field context-dependent (0x0008/0x002a/0x0054),
   +0x42 terminal 0x0220 (jump) / 0x0f00=3840 (fall). Terminal clamp never reached
   in demo (max fall ~0x04ac < 0x0f00) but clamp @0x20695c4 statically verified.
3. [DONE] ARMIPS extraction framework built (extract.py + tools/armips from source, 12 fns verified)
4. [DONE] Action/object-name system mapped (020687b8 name cache, .bac namespace,
   spawner framework 020699xx-02069f1c, action registry 0206cccc, stage player
   02055918, zone-state structs 0x22c4560/0x22c4590)
5. [DONE] Tile/object placement chain mapped (0206aa38 + 0206b244 builders,
   0206b768 dispatcher, 0206bd24 hitbox link, 0206bd5c movement iterator)
6. [DONE] Two-player-entity question RESOLVED: 0x134 = physics player (0204da44);
   0x77c = pause/fix overlay controller (02055918). Entity list mapped
   (0x22b4574 head, 02036e9c alloc, 02037480 init, 02036e04 walker, 020372d0 ctor)
7. [DONE] Action-swap-on-zone-state mapped: 0x22c4560+0x10 = mode word; mode
   transition = 02043c7c mode entity + fade machine 02043b78; mode->game-flow
   dispatcher chain 0204d6d0/0204bf24/0204bed0/0204be48/0204be14 -> 0204bd94;
   in-game setup 0204bf88; act setup 0205aae4 writes player +0x70 action ptr.
   Stage tables 0x2073bd0 (stride 0x22, keyXX.bin files) feed zone-state
   +0x1c/+0x1d/+0x0/+0x20. 15 fns extracted+verified this session (total 49).
8. [DONE] Terminal gameplay loop mapped: 0204bd94 (per-frame) -> stage dispatcher
   0204b470 (table 0x20741f0) + per-frame stage update 0204b528 (table 0x2074184,
   44 object-loader handlers 0x2049c40..0x204a234); 0204bd6c wrapper -> 0204bf88;
   object filename builder 0204b4cc (0x22c448c + "com"); heap free 0x22c4488.
   7 fns extracted+verified this session (total 56).
9. [CORRECTED] Touch state 0x22b65b4 + hardware controller 0x207f02c mapped:
   02033c60 per-frame dispatcher, 0203387c/02033a8c sample/history handlers,
   02033cac/02033d40/02033d60 disable/enable/reset, and 02033de8 init. The old
   "main state dispatcher" name was wrong. Runtime now confirms press/hold/release
   flags and coordinate+1 encoding; the adjacent 0x22b658c table remains partial.
   NOTE: earlier item text used the `0x2037xxx`/`0x200dxxx` address family;
   the correct family is `0x2033xxx` + helpers `0x2009xxx` (item 32).
   Controller functions remain extracted/verified (session total unchanged).
10. [DONE] Player action-state machine fully mapped: 0206c3d0 (812B) update flow
    documented (dead/pending-death, AABB probe 02068e58, partner death, busy gate
    0206c798, +0x62 timer nibbles decrement, +0x68/+0x6c/+0x70 action fn dispatch,
    drift table 0x2077944 via high nibble, action-state obj dispatch 0206c708);
    action-state slots +0x124 (spawner 02069c74)/+0x128/+0x12c (02069934);
    spawners 02069a34/02069b34/02069c74 verified. 5 fns extracted+verified this
    session (total 73).
11. [DONE] HUD/pause overlay callbacks mapped: 0x204ba84 (44B HUD/IRQ sync chain,
    gates on 0x2083e10) + 0x204b95c (272B per-frame score/camera sync, ARM7 bl
    targets 0x233xxxx, score getters 02044690/02044670, 0206c784 bytes->0x22c4eec);
    0204dbb8 (252B) intro countdown cb (skip keys 0xc0b, 0x200->02043c7c mode 5).
    3 fns extracted+verified this session (total 76).
12. [DONE] Object-loader dispatch table 0x2074184 fully mapped: 120 entries,
    10 zone-rows (stride 0xc); rows 0-5 = main zones (mostly bx-lr no-op stubs +
    loaders 0x2049c40/0x204a184/0x204a0d0 writing 0x2369ad0-0x2369ba0); rows 6-9 =
    special/boss with sentinel handlers 0x204afb8(-1)/0x204afc0(0) + resource-file
    strings (narc/mod paths) + loader 0x204a98c (table 0x2074308). Documented in
    Terminal Gameplay Loop section. Verified: 0x204afb8, 0x204afc0, 0x204a98c,
    0x2049c40 (this session, total 80).
13. [DONE] Loaded-code runtime targets dumped via headless hdrv (--dump) during
     attract-demo gameplay: ARM9 main-RAM regions 0x2335000 (32K) + 0x22fa000
     (4K) captured to runtime/*.bin and disassembled (runtime/*.txt). New global
     base 0x236aaa8 (distinct from 0x22b4574 head): accessors 0x23375f8/0x2337618/
     0x2337634 all deref [0x236aaa8+0x10] (=player struct) writing +0x28/+0x67c/
     +0x28c; 0x2336050 stores r1 into [player+0x298]+ clamps 0x12c via 0x2336014.
     0x22fabbc = loaded gameplay dispatcher: gates on [0x22c4560(zone-state)+0x18]
     &0x20, loops 2 entities via 0x206d00c, jump-table (byte 0-10) to handlers
     0x2342e28/0x2342e9c/0x2342ef4/0x2342f14 + 0x204b7ec; type-0xa case writes
     0xff. 0x2335d0c/0x233bc44 operate on lists at 0x236aa8c/0x236aabc.
     Runtime gravity/terminal cross-check confirmed in item 2.
14. [DONE] Full loaded-code space (0x2320000 +0x30000 = 192K) dumped to
     runtime/arm9_loaded_232full.bin + disasm. Handlers mapped:
     - 0x2342e28 = flag-sync: copies +0x324 bit0x1000000 (per-entity) from global
       [g+0x18]+0x330 bit0x800000 into +0x54; state 0x22c4560 gated.
     - 0x2342e9c = spawn-on-cooldown: if !(+0x325), play sfx 0x32/0x80 via
       0x232ecac, run 0x232ee24, self-vel -0x12c0 via 0x234307c, then 0x2336050.
     - 0x2342ef4 = chase move: 0x232f234 + set +0x3c0 = 0x258.
     - 0x2342f14 = burst attack: 0x232f3ec + set +0x3c2 = 0x200.
     - 0x232ee24/0x232f234/0x232f3ec = entity spawners: check +0x3c0/+0x3c2
       cooldown, then ctor 0x20372d0 (size 0x134, slot 4) with callbacks 0x206c3d0
       (player update!)/0x206bef4 etc.
     - 0x234307c = zone-gated apply: [0x22c4560+0x18]&0x20, +0x325 check, sfx
       0x2f/0x80, then 0x232ecac.
     - 0x232ecac = sfx/anim player: ctor 0x20372d0 size 8, slot 4, callbacks
       0x232ec1c/0x232ec74.
     - 0x2330aec = guarded spawn: 0x23270d8 gate + entity[0]==1 check, ctor
       0x20372d0 size 4.
     - 0x2335d0c/0x233bc44 = list walkers: iterate 0x236aa8c/0x236aabc chains via
       0x2006abc (list head), 0x200831c, 0x20067c0 (mem ops).
     - 0x206d00c = entity getter (0x206cef8 lookup +4); 0x206d030 = clear global.
     Loaded layer = second entity system (global 0x236aaa8) driving callbacks
     INTO the same player update fn 0x206c3d0; coexists with static 0x22b4574 head.
15. [DONE] CSV header docs/runtime notes confirmed (probe CSVs: velx/vely at
     +0x28/+0x2a, grav +0x40 context 0x0008/0x002a/0x0054, term +0x42 0x0220/0x0f00,
     flags +0x58 0x00c1/0x80d0, yvel      +0x30, zone 0x22c4560).
16. [DONE] 0x204b7ec = task/object-callback installer: pushes r5 into global
     stack 0x2250454 via 0x201bf1c/0x201bf34, registers cb via 0x201e9cc (caller
     r0,param r5, -1/-1), then byte-field read 0x201be98. Used by loaded
     dispatcher 0x22fabbc to attach per-type update callbacks to the 2 entity
     slots. 0x206bef4 = second callback arg into ctor 0x20372d0 (paired with
     0x206c3d0 player-update). This dispatcher/spawner chain is mapped; the full
     192K loaded image is not semantically complete.
17. [DONE] Scripted attract/demo input path mapped: fcn.02068094 allocates the
     0x20 controller and loads its script; fcn.02067ddc advances 4-byte
     (input-mask, duration) entries, sends masks to fcn.02035134, and drives
     sequence mode 0x22c4ee4. Both functions extracted and ARMIPS-verified
     (total 82).
18. [DONE] Interactive title path reproduced through `hdrv`: keypad Start at
     frames 800–809 opens the menu and a touchscreen tap at `(128,60)` selects
     GAMEPLAY, reaching the Leaf Storm Zone 1 Act 1 intro by frame 1300. The
     reusable script is `scripts/title-to-gameplay.txt`; touch coordinates must
     be written in explicit hexadecimal because `hdrv` currently parses values
     with `%lx`.
19. [DONE/PARTIAL] Title touchscreen consumer identified and prior callback
     theory corrected. ARM7 shared samples remain at 0x027fffaa/0x027fffac;
     static `02033c60 -> {0203387c,02033a8c}` builds touch history/edges at
     0x22b65b4. Loaded title update 0x022c7ac8 consumes press bit0x40 and
     +0x08/+0x0a coordinates, calling AABB hit-test 0x022c678c. Runtime proves
     0x207f02c callback remains no-op 0x02037c5c and table NULL, disproving a
     dynamic callback handoff. Focused loaded blobs/disassemblies preserved.
     RESOLVED by item 32: the ARM7 writer is `0x238c51c` (reader `0x238c0c4`),
     the sampler is `0x2009998`, and the correct ARM9 family is `0x2033xxx`
     with controller helpers `0x2009xxx` (earlier text used `0x2037xxx`/`0x200dxxx`).
20. [DONE/PARTIAL] Post-confirm title chain mapped through creation of the next
     entity. `022c8458` removes marker-`0x1173` entities, disables touch, and
     queues overlay ID 1 plus callback `022d301d` through `0203051c`.
     Main-loop scheduler `020302d8` performs synchronous overlay unload/load,
     invokes the callback once, and clears it. `022d301c` maps global
     `022c4560` null/non-null to constructor argument 0/1 for `022d2e3c`, which
     creates a `0x150c` title-mode entity (update `022d2461`, destroy
     `022d2cbd`, type `0x5000`, marker `0x1173`) and selects one of two loaded
     state callbacks. Adjacent reset helper `022d3044` is mapped but not proven
     to execute. Remaining: map `022d2460`, `022d13f4`, `022d0d34`, and trace
     scheduler globals at runtime across frame 1200 onward.
21. [DONE] Overlay-1 identified and fully static-mapped as the title-menu
     layer. Overlay id=1 loads to RAM `0x022c5c80` (ROM `0x12ec00`). Static
     mapping of the mode-entity subsystem (see items 22-23 and 28-29):
     mode-entity constructor `0x022d2e3c`, update `0x022d2460`/Thumb
     `0x022d2461`, destroy `0x022d2cbc`, entry trampolines `0x022d13f4`/
     `0x022d0d34`, state setter `0x022d1404`, and the full 41-state machine
     with its two linear chains M0 (modes 0/1) and M1 (mode 2). The
     descriptor/state setter is `0x022d2234` (see item 29).
22. [DONE] Mode-entity state machine fully static-mapped (overlay id=1, RAM
     `0x022c5c80`): 41 state transitions covering logo/title/options/demo
     menu screens, entry trampolines `0x022d13f4` (M0 head `0x022d1351`) and
     `0x022d0d34` (M1 head `0x022d0c85`), shared state setter `0x022d1404`
     (writes `[data+0xbd8]`), destroy `0x022d2cbc`, and the two linear state
     chains M0/M1. Struct fields, state flow, and the input->zone-spawn path
     were mapped at the static level.
23. [DONE] GAMEPLAY mode-3 runtime entry proven: mode word `[0x22c4560+0x10]`
     becomes 3 (frame ~1622 interactive; frame 1735 demo/attract). The
     mode-3 entity is created by fixed-ARM9 `0x0204d834` -> `0x0204d6d0(3)`
     (interactive) or `0x02048e9c` -> `0x02048ba4` (demo). `0x0231f050`
     (mode-entity pointer) is never written by any normal flow — the ov1
     mode-entity chain does not run. (See item 28 for the full dead-code
     proof.)
24. [DONE] hdrv rebuilt with expanded telemetry columns (ovact/ovreq/ovload/
     ovcb0/zmod etc.); runtime timeline corrected: gameplay renders from
     ~frame 1622 (white level-load frames 1267-1620; overlay 0 active from
     frame 1267), not frame 1300. ov1 loads at frame 11 with initializer
     `0x022cfb18`.
25. [DONE] Demo/attract path mapped: no-input run past frame 1900 shows demo
     GAMEPLAY entered via `0x02048e9c` (demo trampoline) -> stage loader
     `0x02048ba4` -> mode word `[0x22c4560+0x10]=3` at frame 1735
     (attract=2, overlay 0). `022d301c` did not run; ovcb0 held only
     `0x022cfb19`/`0x02048e9c`.
26. [DONE] Pause/menu path: Start during gameplay pauses via in-place ov0
     pause-overlay entity `0x0204b95c` (slot6 `0x022b4a88`); z20 freezes
     while held, resumes on next Start; no overlay swap, no `022d301c`. Ov0
     code has zero references to the ov1 title chain.
27. [DONE] Interactive title tap chain mapped and scripted: touchscreen tap
     at `(128,60)` selects GAMEPLAY (title is touch-driven; START/D-pad do
     nothing). The GAMEPLAY item is at touch-y ~52-60 (frame-y ~244-252).
     Touch coordinates map directly (tap (128,60) -> game reads (129,61);
     touch Y is bottom-screen 0-191, frame-y = 192+touch-y). Modes-select
     screen (touch-y 132) uses slot6 update `0x022ce29d`; options sub-
     screens (touch-y 88/100) use slot7 update `0x022d85d1`. Interactive
     path: `0x0204d834` -> `0x0204d6d0(3)`.
28. [DONE] `022d301c`/`022d2e3c` RESOLVED: it is a real, fully-functional but
    **unreachable title sub-mode constructor in normal play** — dead code
    reachable only through a self-contained scripted-intro chain in overlay
    1 whose entry is referenced by nothing.
    - The ARM9 side: `0x0203051c(mode, cb)` = `[0x2087cbc]=mode;
      [0x2087cb0]=cb` is the standard mode-transition setter. Fixed-ARM9
      "stub table" at `0x02054df8..` contains one stub per ov1 callback,
      each `ldr ip,=0x203051c; ldr r0,=mode; ldr r1,=cb; bx ip`. The stub
      `0x02054ed8` = `0203051c(1, 0x022d301d)` (exactly what the old docs
      claimed, but reached via a dedicated stub, not via `0x022c8458` as a
      caller — `0x022c8458` is the ov1 target, see below). Siblings include
      `0x02054e4c`=`0203051c(1,0x022cfb19)` (the boot ov1 initializer seen
      as `ovcb0=0x022cfb19` at frame 8) and `0x02054ef4`=`0203051c(1,
      0x022c66e8)`.
    - The ov1 caller: Thumb function `0x022c8458` does `bl 0x2036fdc; bl
      0x2037d40; bl 0x02054ed8` -> schedules `0203051c(1, 0x022d301d)`.
      (The earlier doc's `022c8458` "teardown" disasm was ARM32-misaligned;
      the address was right, the mode was wrong.)
    - `0x022c8458` is installed as an entity update (`[[0x22b4574]+8]`) by
      installers `0x022c6d04` and `0x022c73e8` (identical: set display
      fields, `str 0x022c8458,[ent+8]`, `bl 0x22c7a34`). `0x22c7a34` =
      "set next update" helper: `[ent+8]=new_cb; [ent+0xc]=0`.
    - The installers belong to a chain of scripted attract/intro stages, each
      a counter-driven update (`[ent+0xc]` >= thresholds 5/275/905/1095 ->
      install next stage): `0x022c70b0 -> 0x022c7074 -> 0x022c6f8c ->
      0x022c6ef4 -> 0x022c6e54 -> 0x022c6e04 -> 0x022c6d4c -> 0x022c6d04
      (and 0x022c7430 -> 0x022c73e8) -> 0x022c8458`. The final stage
      schedules `022d301c` ~1095 frames in.
    - DEAD-CODE PROOF: no reference (ARM bl, Thumb bl, or data pointer) to
      ANY chain function (`0x022c70b0..0x022c8458`) exists outside the chain
      region `0x022c6000..0x022c9000` in ov1, nor anywhere in fixed ARM9
      (0x02000000-0x022c0000) or ov0 (0x022c5c80-0x0236a000). The chain
      entry `0x022c70b0` is referenced by nothing. No normal flow (boot,
      title, GAMEPLAY tap, mode-select, options sub-screens, demo/attract,
      pause, START, D-pad) ever sets `ovcb0=0x022d301d`.
    - Forced-execution proof (hdrv gained a `p` script command = poke32):
      script `poke_d301d.txt` writes `[0x2087cbc]=1; [0x2087cb0]=0x022d301d`
      at frame 1500 (exactly what `0203051c(1,0x022d301d)` does). Result:
      `ovcb0`=0x022d301d; **`[0x0231f050]` becomes 0x02090a00** (the
      `022d2e3c` mode-entity store), `[0x02090a00+0]=0` (mode 0, because
      `[0x22c4560]==0`), `mstate`=`[0x02090a00+0x10]` advances
      `0x022d2201 -> 0x022d21d5 -> 0x022d215d -> 0x022d2061` (the 41-state
      chain running). Slot table: slot6 `0x022b4ac0` update `0x022d2461`
      data `0x02090a00` (mode-0 entity, matching `022d2e3c`), slot7
      `0x022b4aa4` update `0x022d224d` (second entity stored at `[data+8]`),
      slot8 `0x022b4a50` update `0x022d0095`. DisPCNT flickers 0x1f50->
      0x0050->0x1f50. The mode-0 screen renders a distinct title/options
      screen (character sprite top screen; two menu rows + red text bottom).
    - Conclusion: the docs' original mechanism was right in spirit —
      `0203051c(1, 0x022d301d)` -> `022d301c` -> `022d2e3c(mode 0/1)` builds
      a title-mode entity — but in the final build it is only wired to a
      dead intro chain. GAMEPLAY itself always uses the fixed-ARM9 mode-3
      path (`0x0204d834` interactive / `0x02048e9c` demo), never
      `022d301c`. The 41-state overlay-1 chain is a leftover title-menu
      mode shell, present but never entered.
29. [DONE] `022d2e3c` MODE-ENTITY FULLY MAPPED (update `0x022d2460`/Thumb
    `0x022d2461`, destroy `0x022d2cbc`, second-entity update `0x022d224c`,
    child-entity spawner `0x022d0158`, shared state setter `0x022d2234`,
    secondary state setter `0x022d1404`, and the runtime-verified state
    chain through its terminal self-destruction). It is a *title-menu /
    intro-presentation mode shell* that runs a scripted intro then returns
    to the title screen — it never reaches gameplay.
    - **Mode-entity data layout** (base = `[0x0231f050]`, e.g. `0x02090a00`,
      size 0x5000): `+0x00` mode (0/1/2), `+0x04` frame counter, `+0x08`
      pointer to second entity (slot `0x022b4aa4`), `+0x0c` sub-struct,
      `+0x10` **state callback** (driven by update via `blx [r4+0x10]`),
      `+0x14` frame counter 2, `+0x930`/`+0x978` two sub-object structs,
      `+0xbb0` init flag, `+0xbb4` input-armed flag, `+0xbb8` input timer,
      `+0xbbc`/`+0xbc4` counter/threshold pair (intro gate), `+0xbd0`
      child-entity slot, `+0xbd8` secondary handler (setter `0x022d1404`),
      `+0xbdc` accumulator, `+0xcbc..0xcf8` more accumulators. Destroy
      frees sub-objects at `+0x454` stride 0x48 (0x12 of them), `+0x964`,
      `+0x9ac`, and 4 objects at `+0xa1c` stride 0x7c.
    - **Shared state setter `0x022d2234`**: `[r0+0x10] = next_state` (or
      default `0x022d1a01` = no-op `bx lr` if arg is 0), clears `[r0+0x14]`.
      This is the "state setter" the prior docs guessed at `0x022d1404`.
      The REAL `0x022d1404` is a *secondary* setter writing `[r0+0xbd8]`
      (default `0x022d0199`) and clearing `[r0+0xbdc]`. Entry trampolines
      `0x022d13f4` (`->0x022d1351`) and `0x022d0d34` (`->0x022d0c85`) are
      the M0/M1 chain heads via `0x022d1404` (as previously mapped).
    - **Update `0x022d2461`** (per-frame): increments `[data+4]` and
      `[data+0x14]`; if `[data+0xbb0]==1` and `[data+0xbb4]==0`, every 0x30
      frames re-renders the two sub-objects at `+0x930`/`+0x978` via
      `0x203344c`/`0x2032d2c`; input handling: reads touch struct
      `0x22b65b4` (`[+0x10]&0x40` = pressed) and calls hit-test `0x22cfb44`
      with touch coords `[+8]/[+0xa]`; also reads keypad `[0x22503e0+4]&8`
      (Start). On a hit/Start it sets `[data+0xbb4]=1`, `[data+0xbb8]=0x3c`
      (60-frame timer), re-renders sub-objects, calls `0x2020204` on
      `[data+0xc]`; then `blx [data+0x10]` (state machine); then updates
      accumulators at `+0xbdc`, `+0xcbc..0xcf8`.
    - **State chain (mode 0), runtime-verified**: `0x022d2201` (set
      sprite/display priority in `[0x208926c+8]`/`[0x2087d44+8]` to 0x10 or
      0xfff0 by mode) -> `0x022d21d5` (mode==2? -> `0x022d20e5` :
      `0x022d215d`) -> `0x022d215d` (wait for both display-priority
      halfwords <= 0) -> `0x022d2061` (increment `[data+0xbbc]`; when
      `[data+0xbbc] >= [data+0xbc4]` (0x23b0 = 9136 frames, ~152s of
      intro), free `[data+8]`, clear display-control regs at
      `0x208922c`/`0x2087d04`, then state `0x022d1ff5`). `0x022d1ff5` calls
      `0x022d0158(2)` = spawn child entity (update `0x022d0095`, size
      0x5000, data `[0]=2`,`[4]=0x80`,`[8]=464`) stored at `[data+0xbd0]`,
      then `0x022d1fd5` (wait `[data+0x14] > 464`) -> `0x022d1ec9` (wait
      `[data+0x14] > 0xf0`, set display regs `0x4000000`/`0x4001000`,
      mode==2 -> `0x022d1905`, else `0x022d1e01`). `0x022d1e01` (non-2):
      fade-in, `0x22d0158(3)` spawn, `0x22cfecc`/`0x22cfe58` ->
      `0x022d1d89` (fade) -> `0x022d1cc5` (wait 0x5a) -> **`0x022d1c9d`
      terminal**: free `[data+8]` object, call `0x022d2e14` which installs
      **`0x022d2db9`** as the container-entity update.
    - **Terminal `0x022d2db9`**: clears `[0x0231f050]` (modep) to 0, plays
      sound via `0x2036fdc`/`0x2037d40`, calls mode jingles
      `0x2059bdc`/`0x2059bcc`/`0x2059b2c` (modes 0/1/2), then
      `0x205a4ec(0)` (deferred-transition dispatcher) which schedules
      `0x2054e30` = `0203051c(1, 0x022cf4c9)` -> returns to the TITLE.
      `0x022cf4c8` clears `[0x2087ccc]` and bit 0x10 of `[0x2087cb8]`.
    - **Runtime proof**: forced run with `p 0x020915bc 0x23b0` (skip the
      9136-frame gate) at frame 1502 advanced the chain exactly as above:
      mstate `0x022d2201 -> 0x022d2061 -> 0x022d1fd5 (f 1600) ->
      0x022d1ec9 (f 2050) -> 0x022d1e01 (f 2300) -> 0x022d1d89 (f 2400) ->
      0x022d1cc5 (f 2850)`; at ~f 2950 modep cleared to 0, mstate 0,
      DisPCNT changed to the attract/title screens, head returned to the
      title entity list, and the mode entity memory was freed (all 0xcccccccc
      at f 5000). **The mode-0 intro runs to completion, self-destructs,
      and returns to the title — it never touches gameplay.** This closes
      P0 tasks 1 and 2.
30. **RESOLVED: the display/VRAM reset helper `0x022d3044` (callable `0x022d3045`) has ZERO callers and is unreachable dead code** (P0 task 4).
    - **Boundary confirmed** (no fallthrough): `0x022d301c` is a self-contained
      Thumb dispatcher, 0x24 bytes, ending with `bx r3` at `0x022d303e`. Its
      literal pool sits at `0x022d3040` (`[0x22d3040]=0x22c4560`) + padding at
      `0x022d3042`. `0x022d3044` begins a **separate** function (`push {lr};
      sub sp, 0x1c`).
    - **Sibling entry points in this block**, all reached ONLY via the fixed
      ARM9 stub table (no in-ov1 callers):
      - `0x022d3010` -> stub ptr `0x022d3011` (stub data word `0x02054ed4`)
        = `movs r0,2; ldr r3,[0x022d3018]=0x022d2e3d; bx r3` -> `0x022d2e3c(2)`
        (M1 chain constructor).
      - `0x022d301c` -> stub ptr `0x022d301d` (stub data word `0x02054ef0`)
        = dispatcher on `[0x22c4560]`; if 0 -> `0x022d2e3c(0)` else
        `0x022d2e3c(1)` (M0 chain constructor).
      - `0x022d3044` -> callable `0x022d3045`: display/VRAM reset helper.
        Clears 8 halfwords at `0x208922c` and 8 at `0x2087d04`, zeroes 6 VRAM
        banks (`0x6000000`/0x80000, `0x6200000`/0x20000, `0x5000000`/0x200,
        `0x5000400`/0x200, `0x5000200`/0x200, `0x5000600`/0x200), masks
        `0x4000000` and `0x4001000` with `0xffffe0ff`.
    - **Caller search results** (exhaustive):
      - No `bl`/`blx` within ov1 to `0x022d3040..0x022d30e0` (whole loaded
        ov1 RAM dump `0x022c5c80..0x0231f01f`, both `overlay1_ram_022c5c80.bin`
        and `ram_overlay1.bin`).
      - No 32-bit literal `0x022d3044`/`0x022d3045` anywhere in ov1, ARM9
        (`ram_lowarm9.bin`), or ov0 (`ram_ov0.bin`).
      - The only PC-relative literal loads landing in `0x022d3044..0x022d30e0`
        are the helper's OWN instructions loading its pool at
        `0x022d30d8/30dc/30e0` (`ldr r0,[0x022d30d8]` at `0x022d304a`, etc.).
      - `0x022d3045` is ABSENT from the fixed ARM9 stub table `0x02054e00..0x02054fec`
        (which exposes `0x022d3011`, `0x022d301d`, `0x022d4175`, `0x022d4fa1`,
        `0x022f9eb4`, `0x02300378`, `0x022e05f0`, `0x022ec345`..`0x022ec381`,
        `0x022e8b09`). Cross-overlay calls to ov1 code can only reach it
        through this table.
    - **Conclusion**: `0x022d3044`/`0x022d3045` is a display/VRAM teardown
      helper belonging to the dead title-intro presentation code (items 28-29).
      It has no static caller, no stub-table entry, and therefore never
      executes in normal play. P0 task 4 closed.
31. **RESOLVED: the LIVE title menu handler is `0x022d8939` (not the dead
    `0x022c7ac8` "TitleMenu_Update"), with items at entity `data+0x24`
    (stride `0x3c`), hit-test `0x022d7a84`, confirm `0x022d8798`,
    options `0x022d85d0`, and touch-options `0x022d86b0`. The whole
    `0x022c7ac8`/`0x022c678c`/`0x022c8480` menu block is dead code**
    (P0 task 5).
    - **Context**: HANDOFF §7/§7.1/§7.2 described the title menu via
      `TitleMenu_Update 0x022c7ac8`, hit-test `0x022c678c` on entity `+0x5b4`,
      and constructor `0x022c8480`. That whole block lies inside the item-28
      dead region `0x022c6000..0x022c9000`; `0x022c8480` has zero callers.
      The live title instead runs entity slots whose updates are
      `0x022d49a1`, `0x022d4815`, `0x022d4539`, `0x022d83fd`, `0x022d84f9`,
      `0x022d8939`, `0x022d8c45` (observed in the f1199 slot list; head
      `0x22b49a8`, slot `0x22b4a88` type `0x10002000` data `0x02090800`
      in the tap run).
    - **Live menu entity** (tap-run f1199, `ram_full_1199b.bin`): data at
      `0x02090800`. Fields: `[data+4]` = selected index (0 at f1199),
      `[data+8]` = frame tick (332), `[data+0xc]` = confirm flag,
      `[data+0x12]` = menu base Y (32), `[data+0x16]` = row pitch (32).
      Menu items are 6 records at `data+0x24 + i*0x3c`, each:
      `+0x00` type (0x09/0x0a/0x0b/0x0c = rows 0-3, 0x00 = logo at x16 y76,
      0x02 = badge at x56 y152), `+0x04` flags, `+0x08` X, `+0x0a` Y.
      Observed rows: (64,32) type 0x09, (64,64) type 0x0a, (64,96) type 0x0b,
      (64,128) type 0x0c; logo (16,76) type 0x00; badge (56,152) type 0x02.
    - **Live hit-test `0x022d7a84(tx, ty, baseY)`**: iterates 4 rows:
      for row i in 0..3, hit if `64 <= tx < 192` AND `baseY+i*32 <= ty <
      baseY+i*32+24`; returns i, else -1. baseY = `[data+0x12]` = 32.
      So rows are Y[32,56), [64,88), [96,120), [128,152). This maps to the
      observed taps: GAMEPLAY ~Y52-60 (row 0), options ~Y88/100 (row 2),
      mode-select ~Y132 (row 3). Note `0x022c678c` (dead) had a different
      signature; the live one is `0x022d7a84`.
    - **Secondary hit `0x022d7a60`**: X in [0x10,0x30)=[16,48) AND Y in
      [0x4c,0x6c)=[76,108) -> selects the logo item (index 4), installing
      `0x022d86b1` (touch-options handler).
    - **Live menu update `0x022d8939`** (Thumb, `push {r4-r7,lr}`): reads
      `[0x22d8b28]=0x22b4574` -> `[[+0x10]]` = menu data; renders 6 items
      via `0x22d7b7c`/`0x22d7ac4`/`0x22d7c04`; then input handling:
      - keypad `0x022503e0`: `[+8]` bit 0x40 -> index-1 (wrap to 3);
        bit 0x80 -> index+1 (wrap to 0); `[+4]` bit 0x01 -> confirm
        (`0x022d8798`); bit 0x02 -> options (`0x022d85d0`, sets index 4).
      - touch `0x022b65b4`: `[+0x10]` press-edge bit 0x40, coords `+0x08/+0x0a`;
        bound-check tx<0x100, ty<0xc0; then `0x022d7a84(tx,ty,[data+0x12])`.
        If row hit -> set `[data+4]=row`, `[data+8]=0`, install update
        `0x022d8798` (confirm), sfx `0x2052104`. Else `0x022d7a60` -> index 4
        + install `0x022d86b1` (options). At end `[data+8]++`.
    - **Confirm handler `0x022d8798`** (installed after select): re-renders,
      waits `[data+8] >= 0x1e` (30 frames), then reads selected item flags
      `[item+4]`: bit2 -> `0x022d7368`; bit0 -> `0x022d7968`; bit1 ->
      `0x022d78f4`; then sets `[data+8]=0, [data+0xc]=0` and reinstalls
      update `0x022d8939`. `0x022d7968`/`0x022d78f4` create the
      "fade"/next-object via `0x20372d0` with update `0x022d84f9` (one of the
      live slot updates) and store `[0x2087d44+8]`/`[0x208926c+8]` color
      pair; `0x022d7368` builds a mode-select palette via `0x2050078`.
    - **Options handler `0x022d85d0`** (keypad-bit2 path): scans items, calls
      `0x2040d14`/`0x2040cf4` on item+8 (hit/flash), same row renderers.
    - **Stub exposure**: live handlers are NOT in the fixed stub table
      `0x02054e00..0x02054fec`; they are reached by the live chain internally.
    - **Conclusion**: P0 task 5's premise (menu item at title entity `+0x5b4`)
      described dead code. The live title menu is `0x022d8939` with items at
      `data+0x24` (stride 0x3c) and hit-test `0x022d7a84`; the GAMEPLAY item
      is row 0 (type 0x09, X64 Y32, hitbox Y[32,56)). P0 task 5 closed.
32. **RESOLVED: complete fixed-ARM9 producer path from shared `0x027fffaa/ac`
    to touch state `0x22b65b4`, plus the ARM7 writer** (P0 task 6).
    - **ARM7 writer (identified)**: fixed-ARM7 `0x238c51c` is the shared-sample
      writer: it calls `0x238c0c4` (shared touchscreen sample reader, returns
      X in r0 / packed Y+status in r1), then `bic/orr 0x6000000` on the sample
      and `strh` X -> `0x027fffaa`, packed Y -> `0x027fffac`, then reports the
      keypad/touch state via `0x238c210`. The touchscreen case-0 handler
      `0x238c594` also writes both shared halfwords (at `0x238c64c`/`0x238c658`),
      invoked from dispatcher `0x238c030` via `0x238c078`. `0x238c0c4` is the
      common reader (also called from `0x238c538`, `0x238c954`, `0x238c9e4`,
      `0x238ca34`, `0x238d374`, `0x238d3b0`, `0x238d3e0`, `0x238d41c`,
      `0x238da5c`, `0x238da98`, `0x238db44`, `0x238ddac`, `0x238e1bc`,
      `0x238e2b4`, `0x238e31c`); `0x238c130` is a loop body inside it.
      `0x238c210` sets keypad/touch flags (50+ callers).
    - **ARM9 sampler `0x2009998`**: reads shared `0x027fffaa` (X) and
      `0x027fffac` (Y) and appends them to the hardware controller `0x207f02c`
      ring buffer: increments the `+0xc` index counter (wraps at `+0x14`
      limit), writes the 8-byte sample slot at `+0x10` table + `idx*8`, and
      touches `+0x34` wait-flag bits. Literals at `0x2009c50` (X) /
      `0x2009c54` (Y); all shared-sample consumers live inside the sampler
      (`0x2009a28`/`0x2009a34` case r2==0; `0x2009b24`/`0x2009b28` case
      r0==0). The touch producers below never read `0x027fffaa/ac` directly;
      they consume the controller ring via the helpers listed next.
    - **ARM9 controller consumers**: `0x2009544` returns `[0x207f02c+0xc]`
      (index); `0x20091a4` copies one 8-byte history slot (checks `+0x30`
      flag); `0x200918c` waits `[+0x36]&mask` clear; `0x2009178` returns
      `[+0x34]&mask`; `0x20095f8`/`0x2009650` poll channel-6 controller state;
      `0x20096f0` installs the controller callback.
    - **ARM9 touch-state producers**:
      - Dispatcher `0x2033c60`: if `[0x22b65b4+0x10]&1` clear, returns; bit1 ->
        `0x203387c` (alternate), else `0x2033a8c` (main).
      - Main `0x2033a8c` (456B): saves current X/Y to `+0x04/+0x06`, polls the
        controller (`0x20095f8`/`0x2009650`), copies a sample slot via
        `0x20091a4`, sets press `0x40`/release `0x80`/contact `0x10`/held `0x20`
        edges, writes `+0x08/+0x0a` (accepted) and `+0x0c/+0x0e` (release)
        history, and sets bit `0x100` when the sampled Y halfword is
        out-of-range/nonzero.
      - Alternate `0x203387c` (calls `0x2033650`): indexes the adjacent
        `0x22b658c` 8-byte table via the controller index `0x2009544` and
        updates the same state fields.
    - **ARM9 init**: input init `0x2033de8` (called from boot `0x202c69c`)
      calls `0x2005fa8` + `0x2009908` (input-subsystem init: zeroes
      `0x207f02c`, registers `0x200d998` as channel-6 callback via `0x20061b4`),
      clears the `0x22b65b4` and `0x22b658c` blocks via `0x20042e8`,
      installs the static controller callback `0x2037c5c` via `0x20096f0`,
      then `0x200985c` + `0x2009714`.
    - **Address-correction note**: earlier analysis attributed the touch
      family to `0x2037xxx` (dispatcher `0x2037c60`, handlers `0x20378c`/
      `0x2037a8c`, disable `0x2037cac`, reset `0x2037d60`, init `0x2037de8`)
      and controller helpers to `0x200dxxx` (`0x200d544`, `0x200d6f0`, ...).
      Those addresses are WRONG (the `0x2037xxx` range is the entity/mode
      callback region, `0x200dxxx` is unrelated IRQ/channel code). The real
      family is `0x2033xxx` + helpers `0x2009xxx`; this document and HANDOFF
      have been corrected.
    - **Conclusion**: the ARM7->ARM9 touch chain is now fully traced and
      statically verified end-to-end; P0 task 6 closed.
 33. [DONE] Touch field semantic mapping — Ghidra headless decompilation
       definitively resolved (P0 task 7):
       - **Ghidra headless decompilation** of ARM9 touch functions completed via
         `/opt/ghidra/support/analyzeHeadless` with BinaryLoader base `0x02000000`.
         Project at `/tmp/sonic-rush-ghidra/SonicRush`; output in
         `/tmp/ghidra_touch_decompiled.txt` (826 lines, 8 functions decompiled).
       - **Touch-state structure `0x22b65b4` (0x28 bytes)** — definitive field
         layout from decompilation + runtime CSV cross-reference:

         | Offset | Size | Name | Semantics | Evidence |
         |--------|------|------|-----------|----------|
         | `+0x00` | 4 | `enable` | Bit0 = touch input enabled | CONFIRMED-STATIC (init `0x2033ddc` clears block) |
         | `+0x04` | 4 | `alternate` | Alternate updater mode flag | CONFIRMED-STATIC (AlternateUpdater checks `[+0x04]==0/1`) |
         | `+0x08` | 4 | `x` | Current X coordinate; `0xffffffff` = no contact | CONFIRMED-RUNTIME (CSV: all `0xffffffff` when no touch) |
         | `+0x0c` | 4 | `y` | Current Y coordinate; `0xffffffff` = no contact | CONFIRMED-RUNTIME (CSV: all `0xffffffff` when no touch) |
         | `+0x10` | 4 | `flags` | Status flags (see bit table below) | CONFIRMED-STATIC + CONFIRMED-RUNTIME |
         | `+0x14` | 4 | `history_count` | Number of coordinate-history entries (observed 1) | CONFIRMED-RUNTIME (always 1 in CSV) |
         | `+0x16` | 2 | `hist_x` | History entry X | INFERRED (8-byte FIFO, see below) |
         | `+0x18` | 2 | `hist_y` | History entry Y | INFERRED |
         | `+0x1a` | 4 | `hist_meta` | History metadata (timestamp/flags) | INFERRED |
         | `+0x1e` | ... | ... | Additional history entries if `history_count` > 1 | INFERRED |

       - **Flags field `+0x10` bit table** (from Sampler `FUN_02009900` +
         AlternateUpdater `FUN_0203383c`):

         | Bit | Name | Meaning | Evidence |
         |-----|------|---------|----------|
         | 0 | `VALID` | Sampler output valid (set by `FUN_02009900` line 236: `*param_3 & 0xfffffffe \| bit0`) | CONFIRMED-STATIC |
         | 1 | `CONTACT` | Screen currently touched (set by sampler line 233: `(shared & 1<<idx) != 0 << 1`) | CONFIRMED-STATIC |
         | 0x10 | `TOUCHED` | Current-frame contact flag (set by sampler via raw sample bit31) | CONFIRMED-STATIC (sampler line 236: `(uVar2 & 0x80000000) != 0`) |
         | 0x20 | `HELD` | Previous-frame contact held (alternate updater checks for edge transitions) | CONFIRMED-STATIC + RUNTIME (re-analysis item 32) |
         | 0x40 | `PRESS_EDGE` | Rising edge: new contact this frame | INFERRED (consistent with DS touch edge conventions) |
         | 0x80 | `RELEASE_EDGE` | Falling edge: contact lost this frame | INFERRED |
         | 0x100 | `OUT_OF_RANGE` | Sampled Y value out of range / nonzero error | INFERRED (sampler checks `Y & 0x7f` for nonzero) |

       - **Sampler (`FUN_02009900` @ `0x02009900`, 440 bytes)** — reads
         shared samples from input subsystem `param_1` at offsets `+0x1180`
         (per-index sample words) and `+0x11c4` (contact bitmask). Writes
         12-byte output to `param_3`:
         - `param_3[0]` (+0x00): flags (bit0=valid from bit31 of sample,
           bit1=contact from bitmask)
         - `param_3[1]` (+0x04): X coordinate — raw bits 0-6 left-shifted 4,
           then scaling factor from bits 8-9 applied (case 0=no shift,
           1=>>1, 2=>>2, 3=>>4)
         - `param_3[2]` (+0x08): pressure — raw bits 16-22 masked to7 bits
         - Two paths: high-bit-set path (direct packed sample) and low-bit
           path (indexed from subsystem `+0x54` per index)

       - **Dispatcher node structure (0x48 bytes, created by `FUN_0203363c` @
         `0x0203363c`)** — intermediary linked-list nodes, NOT the game touch
         state. Key fields:

         | Offset | Name | Role |
         |--------|------|------|
         | `+0x00` | `next` | Next node pointer (linked list) |
         | `+0x04` | `type` | Node type (0=bulk copy, 1=dispatch) |
         | `+0x08` | `index` | Controller sample index |
         | `+0x0c` | `flags` | Node flags (bit1 set by dispatcher) |
         | `+0x10` | `src` | Source address (sampler output / raw data) |
         | `+0x14` | `size` | Data size in bytes |
         | `+0x38` | `src_start` | Source buffer start |
         | `+0x3c` | `src_end` | Source buffer end |
         | `+0x5c` | `buf` | Allocated copy buffer pointer |
         | `+0x60` | `total_size` | Computed total size (`src_end - src_start`) |

       - **Data flow (revised from decompilation):**
         1. ARM7 writer `0x238c51c` writes raw X/Y to shared `0x027fffaa/ac`
         2. Sampler `0x2009998` parses raw samples → 12-byte output (flags, X, pressure)
         3. Controller ring `0x207f02c` stores outputs in table (`+0x10`), indexed by `+0x0c`
         4. Dispatcher `0x2033c60` → `0x2033b98` reads ring, creates 0x48-byte nodes
         5. MainUpdater `0x2033a8c` (`FUN_02033a68`) does bulk aligned memcpy
            (32-byte chunks → 4-byte → 2-byte → 1-byte)
         6. AlternateUpdater `0x203387c` (`FUN_0203383c`) processes type-0 (bulk)
            and type-1 (dispatch) nodes
         7. Game reads touch state at `0x22b65b4` via `0x022c678c` (AABB hit-test)

       - **Runtime cross-reference** (CSV `title1199.csv`):
         - Frames 0-240: `touchflags=0`, `touch0..touchc=0` → init/clear state
         - Frame 300+: `touchflags=1`, `touch0=0xffffffff`, `touch4=0xffffffff`,
           `touch8=0`, `touchc=0`, `touch14=1` → enabled, no contact, history_count=1
         - `0xffffffff` for X/Y = no-contact sentinel (NDS screen is 256x192,
           so any value > 255 indicates no valid touch)

       - **Ghidra caveats**: `FUN_0203383c` (AlternateUpdater) has a pcode error
         at `0x020339b0` ("Unable to resolve constructor") — partial decompilation
         only. Several controller helpers (`0x02009544`, `0x020091a4`, etc.) have
         no Ghidra function — only raw instructions.
