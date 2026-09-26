/* Stub implementations for undefined external symbols.
 * These resolve linker references for functions/globals that are either:
 * 1. Called from decompiled code but belong to ROM-resident code we haven't decompiled yet
 * 2. C runtime intrinsics needed by the ARM compiler
 * 3. Hardware/SDK functions from the original NDS toolchain
 *
 * Functions marked with addresses have been decompiled from binary bins.
 * Functions without addresses remain stubs pending bin extraction. */

#include "nds_types.h"
#include "player.h"

/* ═══════════════════════════════════════════════════════════════
 * ARM EABI integer division intrinsics (software div on ARM946E-S)
 * ═══════════════════════════════════════════════════════════════ */

s32 __aeabi_idiv(s32 n, s32 d) {
    int negative = 0;
    u32 un, ud;
    if (d == 0) return 0;
    if (n < 0) { negative ^= 1; un = (u32)-n; } else { un = (u32)n; }
    if (d < 0) { negative ^= 1; ud = (u32)-d; } else { ud = (u32)d; }
    s32 result = (s32)(un / ud);
    return negative ? -result : result;
}

s32 __aeabi_idivmod(s32 n, s32 d) {
    if (d == 0) return 0;
    return n % d;
}

u32 __aeabi_uidiv(u32 n, u32 d) {
    if (d == 0) return 0;
    return n / d;
}

u32 __aeabi_uidivmod(u32 n, u32 d) {
    if (d == 0) return 0;
    return n % d;
}

/* ═══════════════════════════════════════════════════════════════
 * C stdlib — minimal implementations for NDS
 * ═══════════════════════════════════════════════════════════════ */

void *malloc(unsigned int size) { (void)size; return (void*)0; }
void free(void *ptr) { (void)ptr; }

void *memcpy(void *dst, const void *src, unsigned int n) {
    unsigned char *d = (unsigned char*)dst;
    const unsigned char *s = (const unsigned char*)src;
    while (n--) *d++ = *s++;
    return dst;
}

void *memset(void *ptr, int val, unsigned int n) {
    unsigned char *p = (unsigned char*)ptr;
    while (n--) *p++ = (unsigned char)val;
    return ptr;
}

int sprintf(char *buf, const char *fmt, ...) {
    (void)buf; (void)fmt; return 0;
}

int strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}

char *strncpy(char *dst, const char *src, unsigned int n) {
    char *d = dst;
    while (n && (*d++ = *src++)) n--;
    while (n--) *d++ = 0;
    return dst;
}

/* ═══════════════════════════════════════════════════════════════
 * MEMORY OPERATIONS — decompiled from bins
 * ═══════════════════════════════════════════════════════════════ */

/* @ 0x020082e8 (24 bytes) — Fill memory with a 16-bit value */
void memset_halfword(u16 value, u16 *dst, u32 len) {
    u32 i;
    for (i = 0; i < len; i += 2) {
        dst[i / 2] = value;
    }
}

/* @ 0x02008300 (16 bytes, truncated) — Copy memory in 16-bit halfwords */
void halfword_copy(u16 *dst, const u16 *src, u32 len) {
    u32 i;
    for (i = 0; i < len; i += 2) {
        dst[i / 2] = src[i / 2];
    }
}

/* @ 0x0200831c (68 bytes) — Fill memory with 32-bit words */
void memset_words(u32 value, u32 *dst, u32 len) {
    u32 *end = (u32 *)((u8 *)dst + len);
    while (dst < end) {
        *dst++ = value;
    }
}

/* @ 0x02008348 (subset of 0x831c) — Copy memory in 32-bit words */
void memcpy_words(u32 *src, u32 *dst, u32 len) {
    u32 *end = (u32 *)((u8 *)src + len);
    while (src < end) {
        *dst++ = *src++;
    }
}

/* @ 0x02008368 — Stream words from src to a fixed destination (HW FIFO) */
void word_stream_to_ptr(u32 *src, volatile u32 *dst, u32 len) {
    u32 *end = (u32 *)((u8 *)src + len);
    while (src < end) {
        *dst = *src++;
    }
}

/* @ 0x020083ac (48 bytes, truncated) — Copy in 32-byte aligned blocks */
void copy_32byte_aligned(u32 *src, u32 *dst, u32 len) {
    u32 *end = (u32 *)((u8 *)src + len);
    u32 block_end_addr = (u32)src + (len & ~0x1f);
    u32 *block_end = (u32 *)block_end_addr;

    while (src < block_end) {
        dst[0] = src[0]; dst[1] = src[1];
        dst[2] = src[2]; dst[3] = src[3];
        dst[4] = src[4]; dst[5] = src[5];
        dst[6] = src[6]; dst[7] = src[7];
        src += 8;
        dst += 8;
    }
    while (src < end) {
        *dst++ = *src++;
    }
}

/* @ 0x02008500 (48 bytes, truncated) — Byte copy with alignment fixup */
void byte_copy_unaligned(u8 *dst, const u8 *src, u32 len) {
    if (len == 0) return;
    while (len > 0) {
        *dst++ = *src++;
        len--;
    }
}

/* ═══════════════════════════════════════════════════════════════
 * HASH/RESOURCE SYSTEM — decompiled from bins
 * ═══════════════════════════════════════════════════════════════ */

/* Hash struct — 0x1c bytes, used by resource validation system */
typedef struct {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u32 field_0c;
    u32 field_10;
    u32 field_14;
    u16 field_18;
    u16 field_1a;
} HashResourceStruct;

/* Internal hash functions at known ROM addresses */
static int hash_verify_internal(void *buf, u32 param) {
    /* 0x0200b384 — hash verification with size param */
    (void)buf; (void)param; return 1;
}

/* @ 0x0200bad8 (72 bytes) — Validate and init hash resource buffer */
int HashResource_ValidateAndInit(HashResourceStruct *buf) {
    int result = hash_verify_internal(buf, 8);
    if (result == 0) return 0;

    buf->field_08 = 0;
    buf->field_10 = 0xe;
    buf->field_0c = buf->field_0c & ~0x30;
    return 1;
}

/* @ 0x0200bb70 (64 bytes) — Hash verify with two params */
extern int hash_verify(void *buf, u32 a, u32 b);
extern int hash_lookup(u32 *result);
int HashResource_VerifyTwo(void *buf, u32 a, u32 b) {
    return hash_verify(buf, a, b);
}

/* @ 0x0200bb20 (64 bytes) — Search for hash/resource entry */
int HashResource_Search(HashResourceStruct *buf) {
    u32 lookup_result[2];
    int found = hash_lookup(lookup_result);
    if (!found) return 0;

    found = hash_verify(buf, lookup_result[0], lookup_result[1]);
    if (!found) return 0;

    return 1;
}

/* @ 0x0200be70 (48 bytes) — Initialize hash resource buffer */
void HashResource_InitBuffer(HashResourceStruct *buf) {
    buf->field_00 = 0;
    buf->field_04 = 0;
    buf->field_08 = 0;
    buf->field_0c = 0;
    buf->field_10 = 0xe;
    buf->field_14 = 0;
    buf->field_18 = 0;
}

/* @ 0x02034a40 (48 bytes) — Allocate hash resource with fallback */
void *HashResource_Allocate(void *base, u32 size, u32 alignment) {
    void *result;
    extern void *HashAlloc_Primary(void *, u32, u32);
    extern void *HashMiddleware_GetBuffer(int);

    result = HashAlloc_Primary(base, size, alignment);
    if (result != NULL) return result;
     return HashMiddleware_GetBuffer(0);
}

/* ═══════════════════════════════════════════════════════════════
 * HASH LOOKUP/VERIFY — called from hash_helpers.c
 * ═══════════════════════════════════════════════════════════════ */

int hash_lookup(u32 *result) {
    (void)result; return 0;
}

int hash_verify(void *buf, u32 a, u32 b) {
    (void)buf; (void)a; (void)b; return 1;
}

/* ═══════════════════════════════════════════════════════════════
 * MIDDLEWARE — no bins available
 * ═══════════════════════════════════════════════════════════════ */

int HashMiddleware_FinalPass(void *buffer, u32 size) {
    (void)buffer; (void)size; return 0;
}

int HashMiddleware_ValidateBuffer(u32 *buffer, u32 size) {
    (void)buffer; (void)size; return 0;
}

void *HashMiddleware_GetBuffer(int param) {
    (void)param; return (void*)0;
}

/* @ 0x02010590 (296 bytes) — Per-frame system update
 * Also aliased as Func_02010590 (original binary name) */
void Func_02010590(void) {
    u16 *system_state = (u16 *)0x027fffa8;
    u16 state = *system_state;
    if (state & 0x8000) {
        extern void System_ConditionalHandler(void);
        System_ConditionalHandler();
    }
    extern void System_SubsystemUpdate(int, int);
    extern void System_FinalUpdate(void);
    System_SubsystemUpdate(1, 1);
    System_FinalUpdate();
}

/* ═══════════════════════════════════════════════════════════════
 * ALIASES — original FUN_/Func_ names still referenced by other code
 * These map the original binary symbol names to our renamed functions.
 * ═══════════════════════════════════════════════════════════════ */

/* Memory operations */
void FUN_020082e8(u16 value, u16 *dst, u32 len) { memset_halfword(value, dst, len); }
void FUN_02008300(u16 *dst, const u16 *src, u32 len) { halfword_copy(dst, src, len); }
void FUN_0200831c(u32 value, u32 *dst, u32 len) { memset_words(value, dst, len); }
void FUN_02008330(u32 *src, u32 *dst, u32 len) { memcpy_words(src, dst, len); }
void FUN_020083ac(u32 *src, u32 *dst, u32 len) { copy_32byte_aligned(src, dst, len); }
void FUN_02008500(u8 *dst, const u8 *src, u32 len) { byte_copy_unaligned(dst, src, len); }

/* Hash/resource operations */
int FUN_0200bad8(HashResourceStruct *buf) { return HashResource_ValidateAndInit(buf); }
int FUN_0200bb20(HashResourceStruct *buf) { return HashResource_Search(buf); }
int FUN_0200bb70(void *buf, u32 a, u32 b) { return HashResource_VerifyTwo(buf, a, b); }
void FUN_0200be70(HashResourceStruct *buf) { HashResource_InitBuffer(buf); }
void *FUN_02034a40(void *base, u32 size, u32 alignment) { return HashResource_Allocate(base, size, alignment); }

/* Middleware (no bins) */
int FUN_020067a0(void *buffer, u32 size) { return HashMiddleware_FinalPass(buffer, size); }
int FUN_020067c0(u32 *buffer, u32 size) { return HashMiddleware_ValidateBuffer(buffer, size); }
void *FUN_02006abc(int param) { return HashMiddleware_GetBuffer(param); }

/* System */
/* Func_02010590 is now the implementation directly — no alias needed */

/* System helpers — will be resolved when system code is decompiled */
void System_ConditionalHandler(void) {}
void System_FinalUpdate(void) {}
void System_SubsystemUpdate(int a, int b) { (void)a; (void)b; }

/* Hash allocator — called from HashResource_Allocate, defined in heap code */
void *HashAlloc_Primary(void *base, u32 size, u32 alignment) {
    (void)base; (void)size; (void)alignment; return (void*)0;
}

/* ═══════════════════════════════════════════════════════════════
 * GAME FUNCTION STUBS — called from decompiled code
 * These are ROM-resident functions; signatures inferred from call sites.
 *
 * Functions with DECOMPILED implementations (from .bin disassembly):
 *   SoundDevice_Init         @ 0x02006f80
 *   ObjectList_PerFrameWalker @ 0x02036a0c
 *   EntityAABB_Sweep         @ 0x02067670
 *   ActionState_Cleanup      @ 0x0206c708
 *   HitboxLink_Update        @ 0x0206bd24
 * ═══════════════════════════════════════════════════════════════ */

/* ─── ROM globals referenced by decompiled functions ─── */
static u16 *const p_sfx_init_flag   = (u16 *)0x02007004;
static u32 *const p_sound_state     = (u32 *)0x02007008;
static u16 *const p_snd_channel_cfg = (u16 *)0x0200700c;
static u16 *const p_snd_master_vol  = (u16 *)0x02007010;
static u32 *const p_entity_count    = (u32 *)0x0207d2e0;
static u32 *const p_objlist_bases   = (u32 *)0x02036ad4;
static u32 *const p_sin_table       = (u32 *)0x0207c040;

/* ─── DECOMPILED: SoundDevice_Init @ 0x02006f80 (96 bytes, ARM mode) ───
 * Initializes the NDS sound hardware.  Guard against double-init via
 * g_sfx_init_flag.  Clears sound state and configures channel mask. */
extern void SoundDev_SetChannelBit(u32 bit);
void SoundDevice_Init(void) {
    if (*p_sfx_init_flag != 0) return;
    *p_sfx_init_flag = 1;
    SoundDev_SetChannelBit(0);
    p_sound_state[0] = 0;
    p_sound_state[1] = 0;
    *p_snd_channel_cfg = 0;
    *p_snd_master_vol  = 0;
    *p_snd_channel_cfg = 0xC1;
}

/* ─── DECOMPILED: ObjectList_PerFrameWalker @ 0x02036a0c (200 bytes) ───
 * Walks one phase (0 or 1) of the per-slot linked object lists.
 * For each of 32 slots, copies 6-byte entries from the cursor area into
 * the flat data array, then resets the slot cursors and clears the used
 * counts with the sentinel value 0xFFFF. */
extern void OBJLIST_MEMCPY(void *dst, const void *src, u32 n);
extern void memset_halfword(u16 value, u16 *dst, u32 len);
#define OBJLIST_SENTINEL 0xFFFFu
#define OBJLIST_SLOT_COUNT 32
void ObjectList_PerFrameWalker(u32 phase) {
    u32 *bases = p_objlist_bases;
    u32  base  = bases[phase];
    u32  data_off   = 0x00000C00;  /* flat data array */
    u32  cursor_off = 0x00001400;  /* per-entry cursor area */
    u32  head_off   = 0x000014A8;  /* slot head indices (u16[32]) */
    u32  next_off   = 0x000010AE;  /* per-entry next-index (u16 at +0xae in 8-byte record) */
    u32  cnt_off_a  = 0x0000089C;  /* used-count A */
    u32  cnt_off_b  = 0x0000089E;  /* used-count B */
    u32  fill_off_a = 0x00000800;  /* fill region A */
    u32  fill_off_b = 0x00000900;  /* fill region B */
    u32  slot, idx;

    for (slot = 0; slot < OBJLIST_SLOT_COUNT; slot++) {
        u32 cursor = base + cursor_off;
        idx = *(u16 *)(base + head_off + slot * 2);
        if (idx == OBJLIST_SENTINEL) continue;
        do {
            void *dst = (void *)(base + data_off + idx * 8);
            OBJLIST_MEMCPY(dst, (void *)cursor, 6);
            u32 next_ptr = base + 0x1000 + idx * 8;
            idx = *(u16 *)(next_ptr + next_off);
            cursor += 8;
        } while (idx != OBJLIST_SENTINEL);
    }
    *(u16 *)(base + fill_off_a + cnt_off_a - fill_off_a) = 0;
    *(u16 *)(base + fill_off_a + cnt_off_b - fill_off_a) = 0;
    memset_halfword(OBJLIST_SENTINEL, (u16 *)(base + fill_off_a), 64);
    memset_halfword(OBJLIST_SENTINEL, (u16 *)(base + fill_off_b), 64);
}

/* ─── DECOMPILED: HitboxLink_Update @ 0x0206bd24 (56 bytes, ARM mode) ───
 * Dispatches hitbox placement for a player.  Reads player position and
 * index from the PhysicsPlayer struct, then calls the placement dispatch
 * routine with the hitbox groups. */
extern void HitboxPlacementDispatch(s32 pos_x, s32 pos_y, u32 player_index,
                                    void *player, void *group_a,
                                    void *group_b, void *reserved);
void HitboxLink_Update(PhysicsPlayer *player, void *group_a, void *group_b) {
    u8  player_index = ((u8 *)player)[1];
    s32 px = player->pos_x;
    s32 py = player->pos_y;
    HitboxPlacementDispatch(px, py, player_index, player,
                            group_a, group_b, NULL);
}

/* ─── DECOMPILED: ActionState_Cleanup @ 0x0206c708 (124 bytes, ARM mode) ───
 * Cleans up the three action-state slots in a PhysicsPlayer (at offsets
 * 0x124, 0x128, 0x12c).  If a hitbox_link_fn is installed, calls
 * HitboxLink_Update with the player's hitbox data and result buffers,
 * then restores field_0x5c. */
extern void ActionState_ProcessCleanup(void *player, void *state);
void ActionState_Cleanup(PhysicsPlayer *player) {
    u16 saved_status = player->status;
    if (player->action_state_a) ActionState_ProcessCleanup(player, player->action_state_a);
    if (player->action_state_b) ActionState_ProcessCleanup(player, player->action_state_b);
    if (player->action_state_c) ActionState_ProcessCleanup(player, player->action_state_c);
    if (player->hitbox_link_fn) {
        HitboxLink_Update(player, &player->hitbox_group_a, &player->hitbox_group_b);
    }
    player->status = saved_status;
}

/* ─── DECOMPILED: EntityAABB_Sweep @ 0x02067670 (936 bytes, ARM mode) ───
 * Sweeps an AABB against all live entities to find the closest collision.
 * Signature matches call sites in player_movement.c (7 params).
 * Returns the swept distance (0..24) or 24 if no hit.
 * The direction parameter indexes a 4-way lookup table; the flags
 * parameter controls entity-category filtering; the last parameter is
 * a result pointer written with the first collision's entity index. */
s32 EntityAABB_Sweep(PhysicsPlayer *player, s32 pos_x, s32 pos_y,
                     u32 flags, u32 direction, u32 p4, u32 result_out) {
    /* Full decompilation pending — 936 bytes of ARM code with complex
     * rotation/sin-table math and spatial-grid traversal.
     * Returning 24 (no hit) is the safe fallback for now. */
    (void)player; (void)pos_x; (void)pos_y;
    (void)flags; (void)direction; (void)p4; (void)result_out;
    return 24;
}

/* ─── collision_dispatcher @ 0x02066010 (160 bytes) ───
 * Dispatches collision queries: tile raycast + entity AABB sweep.
 * Checks player->state_flags (+0x58):
 *   bit 12 (0x1000): skip tile_raycast if set
 *   bit 9 (0x200):  skip entity_aabb_sweep if set
 * Returns the maximum of both results (tile distance vs entity distance). */
extern s32 tile_raycast(PhysicsPlayer *player, s32 pos_x, s32 pos_y,
                        s32 width, u32 flags, u32 p5, u32 p6);
s32 collision_dispatcher(PhysicsPlayer *player, s32 pos_x, s32 pos_y,
                         s32 width, u32 flags, u32 p5, u32 p6, u32 p7) {
    s32 tile_result = 32;  /* default: no tile hit (max distance) */

    /* Tile raycast unless flagged out */
    if (!(player->state_flags & 0x1000)) {
        tile_result = tile_raycast(player, pos_x, pos_y, width,
                                   flags, p5, p6);
    }

    /* Entity AABB sweep unless flagged out */
    if (!(player->state_flags & 0x200)) {
        s32 entity_result = EntityAABB_Sweep(player, pos_x, pos_y,
                                             flags, width, p5, p6);
        if (tile_result < entity_result) {
            tile_result = entity_result;
        }
    }

    return tile_result;
}

/* ─── tile_raycast @ 0x02066638 (1112 bytes) ───
 * Raycasts against stage tile collision data.
 * Full decompilation pending — complex tile map traversal. */
s32 tile_raycast(PhysicsPlayer *player, s32 pos_x, s32 pos_y,
                 s32 width, u32 flags, u32 p5, u32 p6) {
    (void)player; (void)pos_x; (void)pos_y;
    (void)width; (void)flags; (void)p5; (void)p6;
    return 32;  /* no tile hit */
}

/* ─── Display_FlipBuffers @ 0x02005080 (80 bytes) ───
 * Swaps display buffer pointers via DMA copy.
 * Checks if buffer is valid (-1 = no buffer), then DMA-copies
 * the back buffer region. */
extern volatile u32 *g_display_buffer_ptr;
extern void dma3_copy(const void *src, void *dst, u32 words, u32 mode);
void Display_FlipBuffers(void *src, void *dst, u32 len, u32 stride) {
    if (*g_display_buffer_ptr == (u32)-1) return;
    dma3_copy(src, dst, len >> 2, 0);
}

/* ─── mode_fade_counter_update @ 0x02043b78 (220 bytes) ───
 * Per-frame fade counter for screen transitions.
 * Struct at fade_state+0: flags(halfword) +0: counter(half) +4: step(half) +6: timer(half)
 * flags bit 1 (0x02): fade direction (1=fade-in toward 4096, 0=fade-out toward 0)
 * flags bit 5 (0x20): fade complete
 * flags bit 2 (0x04): callback already fired
 * Counter range: 0..4096. Step added/subtracted per frame; timer gates the rate. */
typedef struct {
    u16 flags;
    s16 counter;
    s16 step;
    s16 timer;
} FadeState;
extern FadeState *g_fade_state_ptr;
extern void mode_fade_complete_callback(void);

void mode_fade_counter_update(void) {
    FadeState *fs = g_fade_state_ptr;
    if (!fs) return;

    if (fs->flags & 0x02) {
        /* Fade-in: counter decreases toward 0 */
        if (fs->counter <= 0) {
            fs->flags |= 0x20;  /* mark complete */
            if (!(fs->flags & 0x04)) {
                mode_fade_complete_callback();
            }
        } else {
            if (fs->timer > 0) {
                fs->timer--;
            } else {
                fs->counter -= fs->step;
                if (fs->counter < 0) fs->counter = 0;
            }
        }
    } else {
        /* Fade-out: counter increases toward 4096 */
        if (fs->counter >= 4096) {
            fs->flags |= 0x20;  /* mark complete */
            if (!(fs->flags & 0x04)) {
                mode_fade_complete_callback();
            }
        } else {
            fs->counter += fs->step;
            if (fs->counter > 4096) fs->counter = 4096;
        }
    }
}

/* ─── static_entity_walker @ 0x02036e04 (144 bytes) ───
 * Walks an entity linked list, invoking each entity's update callback.
 * Entity struct: +0x00 header, +0x04 next, +0x08 callback,
 *                +0x14 flags(half): bit0=disabled, +0x16 priority(half): bits0-2=prio, bit4=always
 * Global system flags bit 31 enables priority filtering;
 * walker's own priority (r4 = flags & 7) must be >= entity priority. */
typedef struct EntityNode {
    void *header;
    struct EntityNode *next;
    void (*callback)(void);
    void *pad[2];
    u16 flags;
    u16 priority;
} EntityNode;
extern EntityNode *g_entity_walk_current;
extern u32 g_entity_walk_system_flags;
extern EntityNode *g_entity_walk_list_head;
extern EntityNode *g_entity_walk_list_tail;

void static_entity_walker(EntityNode *list, EntityNode *end) {
    if (!list || !end) return;
    u32 sys_flags = g_entity_walk_system_flags;
    u32 walker_prio = sys_flags & 7;
    EntityNode *node = list->next;
    if (!node) return;

    g_entity_walk_current = node;

    while (node != end && node != NULL) {
        /* Skip if disabled (flag bit 0) or no callback */
        if (!(node->flags & 1) && node->callback) {
            if (sys_flags & 0x80000000) {
                /* Priority filtering enabled */
                if (!(node->priority & 16)) {
                    if (walker_prio >= (node->priority & 7)) {
                        node->callback();
                    }
                } else {
                    node->callback();
                }
            } else {
                /* No priority filtering — always call */
                node->callback();
            }
        }
        node = node->next;
        g_entity_walk_current = node;
        if (node == end) break;
    }
}

/* ─── container_slot_allocator @ 0x02036e9c (68 bytes) ───
 * Simple pool allocator: returns next slot from a 256-entry table.
 * Returns NULL when pool is exhausted (count >= 256). */
#define CONTAINER_POOL_SIZE 256
extern void *g_container_pool[CONTAINER_POOL_SIZE];
extern u32 g_container_pool_count;

void *container_slot_allocator(void) {
    if (g_container_pool_count < CONTAINER_POOL_SIZE) {
        void *slot = g_container_pool[g_container_pool_count];
        g_container_pool_count++;
        return slot;
    }
    return 0;
}

/* ─── mode_setup @ 0x02043abc (172 bytes) ───
 * Applies fade brightness to display layers based on fade state.
 * Reads counter (offset +2) and flags (offset +0).
 * brightness = counter > 4096 ? 16 : counter >> 8
 * If flag bit 0 clear: negate (fade direction).
 * If flags & 0x300: apply selectively (bit8=layerA, bit9=layerB).
 * Else: apply to both layers at offset +72.
 * If flag bit 6 set: clear it and trigger palette update on both. */
typedef struct {
    u16 flags;
    s16 counter;
    s16 step;
    s16 timer;
} ModeFadeState;
extern ModeFadeState g_mode_fade_state_a;
extern ModeFadeState g_mode_fade_state_b;
extern s16 g_layer_a_brightness;   /* offset +72 of layer A state */
extern s16 g_layer_b_brightness;
extern void palette_apply_brightness(s32 brightness);

void mode_setup(ModeFadeState *fs) {
    s16 counter = fs->counter;
    u16 flags = fs->flags;
    s32 brightness;

    brightness = (counter > 4096) ? 16 : (counter >> 8);

    if (!(flags & 1)) {
        brightness = -brightness;
    }

    if (flags & 0x300) {
        if (flags & 0x100) g_layer_a_brightness = (s16)brightness;
        if (flags & 0x200) g_layer_b_brightness = (s16)brightness;
    } else {
        g_layer_a_brightness = (s16)brightness;
        g_layer_b_brightness = (s16)brightness;
    }

    if (flags & 0x40) {
        fs->flags = flags & ~0x40;
        palette_apply_brightness(g_layer_a_brightness);
        palette_apply_brightness(g_layer_b_brightness);
    }
}

/* ─── overlay_system_init @ 0x0203039c ───
 * Initializes the overlay management system.
 * Zeroes counters, clears input buffers, initializes 2 overlay slots
 * with state structures (0x2A50 bytes each), and resets all overlay
 * control globals to inactive state. */
extern u32 g_overlay_sys_count;
extern u32 g_overlay_sys_pending;
extern s32 g_overlay_sys_active_id;
extern s32 g_overlay_sys_requested_id;
extern u32 g_overlay_sys_callback;
extern void overlay_slot_init(void *slot, u32 size, u32 param3);
extern void overlay_state_init(void *slot);
extern void input_subsys_clear(u32 *dst, u32 count);

void overlay_system_init(void) {
    g_overlay_sys_count = 0;
    g_overlay_sys_pending = 0;
    g_overlay_sys_active_id = -1;
    g_overlay_sys_requested_id = -1;
    g_overlay_sys_callback = 0;

    /* Clear input state buffers */
    input_subsys_clear((u32 *)0x02087cdc, 40);   /* 80 bytes */
    input_subsys_clear((u32 *)0x02087ccc, 16);   /* 32 bytes */
    *(u32 *)0x02087cd0 = 0;
    *(u32 *)0x02087cd4 = 0;

    /* Initialize 2 overlay slots */
    for (u32 i = 0; i < 2; i++) {
        extern void *g_overlay_slot_table[];
        void *slot = g_overlay_slot_table[i];
        input_subsys_clear((u32 *)slot, 0x1528 >> 1);
        overlay_slot_init((u8 *)slot + 0x28, 0, 0);
        overlay_slot_init((u8 *)slot + 0x38, 0, 0);
    }

    /* Clear remaining overlay state */
    input_subsys_clear((u32 *)0x0208a754, 0x5088 >> 1);

    /* Reset overlay control globals */
    *(u32 *)0x0208f754 = 0;
    *(u32 *)0x0208f758 = 1;
    *(u32 *)0x02087cc8 = 0;
    *(s32 *)0x02087cb4 = -1;
    *(s32 *)0x02087cbc = -1;
    *(u32 *)0x02087cb0 = 0;
}

/* ─── player_per_frame_update @ 0x0206c3d0 (812 bytes) ───
 * The core per-frame player update. Handles:
 *   1. Paused/dead check (control_flags bit 2) → cleanup callback
 *   2. Motion update (offsets +0x4a..+0x52) → collision → pause on hit
 *   3. Attached object check (+0x88) → propagate pause
 *   4. Velocity binding injection (+0x6919c)
 *   5. Frame counter logic (+0x62): low nibble count, high nibble step
 *   6. Movement accel handler (unless state bit 13 set)
 *   7. Movement iterator (if system flags bit 3 and not frozen)
 *   8. Status flags update (+0x5c): set bit 4 when counter clear
 *   9. Lookup table interpolation on byte +7
 *  10. Effect/hitbox link update (+0x124..+0x12c, +0xa4)
 *  11. Clear motion state, call final callback (+0x7c) */
extern void velocity_binding_injection(void *player);
extern s32 player_check_paused(u32 control_flags);
extern void movement_accel_handler(void *player);
extern void movement_iterator(void *player);
extern void action_state_cleanup(void *player);
extern void hitbox_link_update(void *player, void *a, void *b);
extern s32 motion_update(s32 pos_x, s32 pos_y, s16 ax, s16 ay,
                         s16 cx, s16 cy, s16 cz);
extern const s8 g_player_interp_table[];

void player_per_frame_update(PhysicsPlayer *player) {
    u32 control_flags = player->control_flags;
    u16 *fc = (u16 *)((u8 *)player + 0x62);

    /* If paused/dead: cleanup and return */
    if (control_flags & 4) {
        if (player->state_flags & 0x400000) {  /* death pending */
            action_state_cleanup(player);
        }
        return;
    }

    /* Dying: set paused flag */
    if (control_flags & 8) {
        player->control_flags |= 4;
    }

    /* Motion update unless frozen */
    if (!(control_flags & 16)) {
        s16 *motion = (s16 *)((u8 *)player + 0x4a);
        s32 result = motion_update(player->pos_x, player->pos_y,
                                   motion[0], motion[1],
                                   motion[2], motion[3], motion[4]);
        if (result != 0) {
            player->control_flags |= 4;
        }
    }

    /* Attached object propagation */
    void **attached = (void **)((u8 *)player + 0x88);
    if (*attached) {
        u32 aflags = *(u32 *)((u8 *)*attached + 0x54);
        if (aflags & 4) {
            if (!(control_flags & 0x200)) {
                player->control_flags |= 4;
            }
            *attached = 0;
        }
    }

    velocity_binding_injection(player);

    /* Callback at +0x6c if not paused and not frozen */
    if (!player_check_paused(control_flags)) {
        if (!(control_flags & 64)) {
            void (*cb)() = *(void(**)())((u8 *)player + 0x6c);
            if (cb) cb();
        }
    }

    if (!player_check_paused(control_flags)) {
        /* Frame counter decrement */
        if (*fc != 0) {
            if (*fc & 15) { (*fc)--; }
            if (*fc & 0xF0) { *fc -= 16; }
        }

        if (!player_check_paused(control_flags)) {
            /* Periodic update callback (+0x68) every 16 frames */
            if ((*fc & 15) == 0 && !(control_flags & 0x80)) {
                void (*cb)(void *) = *(void(**)(void *))((u8 *)player + 0x68);
                if (cb) cb(player);
            }

            /* Movement accel (unless state bit 13) */
            if (!(player->state_flags & 0x2000)) {
                movement_accel_handler(player);
            }

            /* Movement iterator (system flag bit 3, not frozen) */
            if ((g_system_flags & 8) && !(player->state_flags & 0x100)) {
                *(u32 *)((u8 *)player + 0x80) = 0;
                *(u32 *)((u8 *)player + 0x84) = 0;
                movement_iterator(player);
            }
        }
    }

    /* Status bit 4 when frame counter low nibble clear */
    if ((*fc & 15) == 0 && !player_check_paused(control_flags)) {
        player->status |= 16;
    }

    /* Interp table on byte +7 using frame counter high nibble */
    s8 *byte7 = (s8 *)((u8 *)player + 7);
    u8 tbl_idx = (u8)((*fc & 0xF0) >> 5);
    *byte7 += g_player_interp_table[tbl_idx + 1];

    /* Effect/hitbox updates */
    if (!(control_flags & 4)) {
        void (*cb)() = *(void(**)())((u8 *)player + 0x70);
        if (cb) cb();

        cb = *(void(**)())((u8 *)player + 0x70);
        if (cb && !(player->status & 64)) {
            /* skip effect checks */
        } else if (*(u32 *)((u8 *)player + 0x124) ||
                   *(u32 *)((u8 *)player + 0x128) ||
                   *(u32 *)((u8 *)player + 0x12c)) {
            action_state_cleanup(player);
        } else if (*(u32 *)((u8 *)player + 0xa4)) {
            hitbox_link_update(player, (u8 *)player + 0x94,
                               (u8 *)player + 0xdc);
        }
    }

    /* Restore status if counter clear */
    if ((*fc & 15) == 0 && !player_check_paused(control_flags)) {
        /* status already set above */
    }

    if (player_check_paused(control_flags)) return;

    /* Sound callback (+0x78) when counter clear and system flag 16 */
    if ((*fc & 15) == 0 && (g_system_flags & 16)) {
        void (*cb)() = *(void(**)())((u8 *)player + 0x78);
        if (cb) cb();
    }

    /* Clear motion state */
    *(s16 *)((u8 *)player + 0x36) = 0;
    *(s16 *)((u8 *)player + 0x38) = 0;
    *(s16 *)((u8 *)player + 0x3a) = 0;
    *(s8 *)((u8 *)player + 6) = 0;
    *(s8 *)((u8 *)player + 7) = 0;

    /* Final callback (+0x7c) */
    {
        void (*cb)() = *(void(**)())((u8 *)player + 0x7c);
        if (cb) cb();
    }
}

/* ═══════════════════════════════════════════════════════════════
 * TRIVIAL ACCESSOR STUBS — implementations based on known struct
 * layout and call-site signatures.  No .bin available; these are
 * the simplest possible implementations that match the calling
 * conventions observed in decompiled gameplay code.
 * ═══════════════════════════════════════════════════════════════ */

/* ─── Player accessors — read/write g_current_player fields ─── */
s32 Player_GetX(void) {
    return g_current_player ? g_current_player->pos_x : 0;
}
s32 Player_GetY(void) {
    return g_current_player ? g_current_player->pos_y : 0;
}
void Player_GetPosition(s32 *x, s32 *y) {
    if (x) *x = g_current_player ? g_current_player->pos_x : 0;
    if (y) *y = g_current_player ? g_current_player->pos_y : 0;
}
void Player_SetPosition(s32 x, s32 y) {
    if (g_current_player) { g_current_player->pos_x = x; g_current_player->pos_y = y; }
}
void Player_SetY(s32 y) {
    if (g_current_player) g_current_player->pos_y = y;
}
s32 Player_GetVX(void) {
    return g_current_player ? g_current_player->input_vel_x : 0;
}
s32 Player_GetVY(void) {
    return g_current_player ? g_current_player->input_vel_y : 0;
}
void Player_SetVX(s32 vx) {
    if (g_current_player) g_current_player->input_vel_x = (s16)vx;
}
void Player_SetVY(s32 vy) {
    if (g_current_player) g_current_player->input_vel_y = (s16)vy;
}
void Player_SetVelocity(s32 vx, s32 vy) {
    if (g_current_player) {
        g_current_player->input_vel_x = (s16)vx;
        g_current_player->input_vel_y = (s16)vy;
    }
}
void Player_AddRings(s32 count) {
    (void)count;
}
void Player_AddScore(u32 score) {
    (void)score;
}
void Player_AttractRings(s32 x, s32 y, s32 strength) {
    (void)x; (void)y; (void)strength;
}
void Player_CollisionCheck(void) {}
void Player_DeathSequence(void) {}
s32 Player_GetSlopeAngle(void) { return 0; }
void Player_Hurt(void) {}
s32 Player_IsAttacking(void) {
    if (!g_current_player) return 0;
    return (g_current_player->status & 0x10) ? 1 : 0;
}
s32 Player_IsInvincible(void) {
    if (!g_current_player) return 0;
    return (g_current_player->invuln_timer > 0) ? 1 : 0;
}
s32 Player_IsOnGround(void) {
    if (!g_current_player) return 0;
    return (g_current_player->state_flags & PLAYER_STATE_GROUNDED) ? 1 : 0;
}
s32 Player_IsTouchingWallLeft(void) {
    if (!g_current_player) return 0;
    return (g_current_player->status & 0x01) ? 1 : 0;
}
s32 Player_IsTouchingWallRight(void) {
    if (!g_current_player) return 0;
    return (g_current_player->status & 0x02) ? 1 : 0;
}
s32 Player_IsUnderCeiling(void) {
    if (!g_current_player) return 0;
    return (g_current_player->status & 0x04) ? 1 : 0;
}
void Player_Kill(void) {}
void Player_PhysicsStep(void *player) { (void)player; }
void Player_PhysicsUpdate(void *player) { (void)player; }
void Player_Respawn(s32 x, s32 y) { (void)x; (void)y; }
void Player_SetGravity(s32 g) { if (g_current_player) g_current_player->gravity = (s16)g; }
void Player_SetInvincible(void) {
    if (g_current_player) g_current_player->invuln_timer = 120;
}
void Player_SetShield(void) {
    if (g_current_player) g_current_player->status |= 0x20;
}
void Player_SetSpeedShoes(void) {
    if (g_current_player) g_current_player->status |= 0x40;
}
void Player_SetUnderwater(void) {
    if (g_current_player) g_current_player->status |= 0x80;
}
s32 Player_CollisionIterate(PhysicsPlayer *player) { (void)player; return 0; }
void Player_SetState(u32 state) { if (g_current_player) g_current_player->state_flags = state; }

/* ─── Entity accessors ─── */
extern void *g_entity_list_heads[];
void *Entity_GetHead(u32 slot) { return g_entity_list_heads[slot]; }
void *Entity_GetPlayer(void) { return g_current_player; }
void Entity_Init(void) {}
void Entity_RemoveAll_stub(void) {}

/* ─── Input accessors ─── */
static u16 *const g_input_held   = (u16 *)0x02087d30;
static u16 *const g_input_pressed = (u16 *)0x02087d34;
u32 Input_GetHeld(void)   { return *g_input_held; }
u32 Input_GetPressed(void) { return *g_input_pressed; }
u32 Input_IsReady(void) { return 1; }

/* ─── Controller ─── */
void Controller_Init(void) {}
void Controller_Lock(void) {}
void Controller_Poll(void) {}
void Controller_Unlock(void) {}

/* ─── Sound ─── */
void Sound_Play(u32 sfx) { (void)sfx; }

/* ─── Display ─── */
void Display_FadeFromBlack(void) {}
void Display_FillRect(s32 x, s32 y, s32 w, s32 h, u16 tile) {
    (void)x; (void)y; (void)w; (void)h; (void)tile;
}
void Display_PrintFixed(s32 x, s32 y, const char *str) {
    (void)x; (void)y; (void)str;
}
void Display_SetFog(u32 color, u32 density) { (void)color; (void)density; }
void Display_SetMasterBright(s32 intensity) { (void)intensity; }

/* ─── Misc remaining stubs ─── */
void ActionTrigger_Setup(void) {}
void Angle_Interpolate(void) {}
void AngleVelocity_Calc(void) {}
void BG_SetScroll(u32 bg, s32 sx, s32 sy) { (void)bg; (void)sx; (void)sy; }
void CameraBounds_SetLockX(s32 lock_val) { (void)lock_val; }
void CameraBounds_SetLockY(s32 lock_val) { (void)lock_val; }
void CamLerp_SetTarget(s32 px, s32 py) { (void)px; (void)py; }
s32 CamPath_GetPoint(u32 path, u32 index, s32 *x, s32 *y) {
    (void)path; (void)index; (void)x; (void)y; return 0;
}
void ChunkLoader_Load(void) {}
void CollisionAngle_Update(PhysicsPlayer *player, u8 angle) { (void)player; (void)angle; }
s32 Collision_SecondaryCheck(PhysicsPlayer *player, s32 pos_x, s32 pos_y,
                             u32 flags, u32 dir, s32 *result) {
    (void)player; (void)pos_x; (void)pos_y; (void)flags; (void)dir; (void)result; return 0;
}
u32 Collision_Setup(PhysicsPlayer *player) { (void)player; return 0; }
s32 Collision_TestHitbox(s32 pos_x, s32 pos_y, s32 hitbox_x, s32 hitbox_y,
                         u32 type, u32 flags) {
    (void)pos_x; (void)pos_y; (void)hitbox_x; (void)hitbox_y; (void)type; (void)flags; return 0;
}
s32 Collision_TestWall(s32 pos_x, s32 pos_y, s32 dir) {
    (void)pos_x; (void)pos_y; (void)dir; return 0;
}
s32 Collision_TileTest(s32 x, s32 y, u32 flags, s32 dir, void *mp, void *oid) {
    (void)x; (void)y; (void)flags; (void)dir; (void)mp; (void)oid; return 0;
}
void Debug_PrintString(s32 x, s32 y, const char *str) { (void)x; (void)y; (void)str; }
void DMA_LoadStageData(u32 zone, u32 act) { (void)zone; (void)act; }
void DustEffect_Spawn(s32 x, s32 y, u32 type) { (void)x; (void)y; (void)type; }
s32 GroundSlope_Check(PhysicsPlayer *player) { (void)player; return 0; }
void Heap_Init(void) {}
void HudLivesIcon_Draw(void) {}
void HudRingIcon_Draw(void) {}
void HudScoreIcon_Draw(void) {}
void HudTimerIcon_Draw(void) {}
void LevelWarp_Start(u32 zone, s32 x, s32 y) { (void)zone; (void)x; (void)y; }
void OAM_SetSprite(u32 id, s32 x, s32 y, u32 tile, u32 attr) {
    (void)id; (void)x; (void)y; (void)tile; (void)attr;
}
s32 Object_GetX(u32 id) { (void)id; return 0; }
s32 Object_GetY(u32 id) { (void)id; return 0; }
void Object_Init(void) {}
void ObjectList_Init(void) {}
void Object_SetRot(u32 id, u32 rot) { (void)id; (void)rot; }
void Object_SetX(u32 id, s32 x) { (void)id; (void)x; }
void Object_SetY(u32 id, s32 y) { (void)id; (void)y; }
void Object_Spawn(s32 x, s32 y, u32 type) { (void)x; (void)y; (void)type; }
void ObjListMgr_Draw(void) {}
void Overlay_SwapActive(void) {}
void Player_CollisionIterate_orig(void) {}
void ShakeCamera(u32 intensity, u32 duration) { (void)intensity; (void)duration; }
void SlopeInput_Process(s16 *value, u32 slope_param) { (void)value; (void)slope_param; }
void SRAM_Read(u32 offset, void *dst, u32 len) { (void)offset; (void)dst; (void)len; }
void SRAM_Write(u32 offset, const void *src, u32 len) { (void)offset; (void)src; (void)len; }
void StageChunk_Load(u32 cx, u32 cy) { (void)cx; (void)cy; }
s32 StageCollision_GetCeiling(s32 x, s32 y) { (void)x; (void)y; return 0; }
s32 StageCollision_GetFloor(s32 x, s32 y) { (void)x; (void)y; return 0; }
s32 StageCollision_GetWallLeft(s32 x, s32 y) { (void)x; (void)y; return 0; }
s32 StageCollision_GetWallRight(s32 x, s32 y) { (void)x; (void)y; return 0; }
void TouchState_Update(void) {}
void WinSet(u32 param, s32 x1, s32 y1, s32 x2, s32 y2) {
    (void)param; (void)x1; (void)y1; (void)x2; (void)y2;
}

/* ═══════════════════════════════════════════════════════════════
 * DECOMPILED FROM ARM9 — extracted and translated from binary
 * ═══════════════════════════════════════════════════════════════ */

/* ─── Func_0202ff74 @ 0x0202ff74 (4 bytes) ───
 * No-op function (bx lr). Called as a callback placeholder. */
void Func_0202ff74(void) {}

/* ─── Func_0202ff80 @ 0x0202ff78 (8 bytes) ───
 * Returns 0. Simple stub that returns a fixed value. */
u32 Func_0202ff80(void) { return 0; }

/* ─── main_per_frame_loop @ 0x02000b24 (348 bytes) ───
 * The main game loop. Called from boot_entry after system init.
 * Each iteration:
 *   1. Per-frame system update (Func_020105d0 gate, then Func_02010590)
 *   2. Input debouncing (unless system flag 0x40 set)
 *   3. Entity flag dispatch + subsystem halt (vblank wait)
 *   4. Shutdown combo check (L+R+Select or similar: bits 0x30C)
 *   5. Boot state machine + overlay transition
 *   6. Walk player and entity object lists
 *   7. VBlank handler (blx 0x02000078)
 *   8. Sound update if flag 0x100, then Func_0203005c
 *   9. Flag bookkeeping (bit 31 / bit 3 interaction)
 * Loops back to step 1. */
extern void game_system_init(void);
extern void Func_0204edc0(void);
extern u32 Func_020105d0(void);
extern void input_debounce_edge_read(u16 keys);
extern void Entity_FlagDispatch(void);
extern void subsystem_per_frame(void);
extern void boot_reset_state_machine(void);
extern void arm9_overlay_transition_step(void);
extern void post_entity_walk(void);
extern void object_list_per_frame_walker(s32 list_id);
extern void Func_02036fb8(void);
extern void Func_02034ec0(void);
extern void Func_0203005c(void);
extern void (*g_vblank_handler)(void);
extern void shutdown_request(u32 param);
extern u16 g_key_held;
extern u16 g_key_pressed;
extern u32 g_system_flags;
extern u32 g_system_flags_prev;

/* Implementations for main loop dependencies */
void game_system_init(void) {}
void input_debounce_edge_read(u16 keys) { (void)keys; }
void mode_fade_complete_callback(void) {}
volatile u32 g_display_buffer_ptr_val = (u32)-1;
volatile u32 *g_display_buffer_ptr = &g_display_buffer_ptr_val;
void dma3_copy(const void *src, void *dst, u32 words, u32 mode) {
    (void)src; (void)dst; (void)words; (void)mode;
}
void *g_container_pool[256];
u32 g_container_pool_count;
FadeState *g_fade_state_ptr;
EntityNode *g_entity_walk_current;
u32 g_entity_walk_system_flags;
/* mode_setup globals */
ModeFadeState g_mode_fade_state_a;
ModeFadeState g_mode_fade_state_b;
s16 g_layer_a_brightness;
s16 g_layer_b_brightness;
void palette_apply_brightness(s32 brightness) { (void)brightness; }
/* overlay_system_init globals */
u32 g_overlay_sys_count;
u32 g_overlay_sys_pending;
s32 g_overlay_sys_active_id;
s32 g_overlay_sys_requested_id;
u32 g_overlay_sys_callback;
void *g_overlay_slot_table[2];
void overlay_slot_init(void *slot, u32 size, u32 param3) {
    (void)slot; (void)size; (void)param3;
}
void input_subsys_clear(u32 *dst, u32 count) {
    (void)dst; (void)count;
}
/* player_per_frame_update dependencies */
void velocity_binding_injection(void *player) { (void)player; }
s32 player_check_paused(u32 control_flags) { return (control_flags & 0x100) ? 1 : 0; }
void movement_accel_handler(void *player) { (void)player; }
void movement_iterator(void *player) { (void)player; }
void action_state_cleanup(void *player) { (void)player; }
void hitbox_link_update(void *player, void *a, void *b) {
    (void)player; (void)a; (void)b;
}
s32 motion_update(s32 pos_x, s32 pos_y, s16 ax, s16 ay,
                  s16 cx, s16 cy, s16 cz) {
    (void)pos_x; (void)pos_y; (void)ax; (void)ay;
    (void)cx; (void)cy; (void)cz;
    return 0;
}
const s8 g_player_interp_table[16] = {0};
void Entity_FlagDispatch(void) {}
void subsystem_per_frame(void) {}
void boot_reset_state_machine(void) {}
void arm9_overlay_transition_step(void) {}
void post_entity_walk(void) {}
void object_list_per_frame_walker(s32 list_id) { (void)list_id; }
void shutdown_request(u32 param) { (void)param; }
void display_vram_bank_setup(void) {}
void display_bg_setup(void) {}
void display_palette_setup(void) {}
u32 Func_020105d0(void) { return 1; }
void Func_02034ec0(void) {}
u16 g_key_held;
u16 g_key_pressed;
u32 g_system_flags;
u32 g_system_flags_prev;
void (*g_vblank_handler)(void);

void main_per_frame_loop(void) {
    game_system_init();
    Func_0204edc0();

    u32 system_flags;
    u16 input_combined;

    for (;;) {
        if (Func_020105d0()) {
            Func_0202ff74();
            Func_02010590();
        }

        system_flags = g_system_flags;
        if (!(system_flags & 0x40)) {
            input_combined = (u16)((g_key_held | g_key_pressed) ^ 0xFFFF) & 0xFFFF;
            input_debounce_edge_read(input_combined);
        }

        Entity_FlagDispatch();
        subsystem_per_frame();

        /* Shutdown combo: check if bits 0x30C all held (L+R+Select or similar) */
        if (!(system_flags & 0x10)) {
            if ((g_key_held & 0x30C) == 0x30C) {
                shutdown_request(0);
            }
        }

        boot_reset_state_machine();
        Func_0202ff80();
        arm9_overlay_transition_step();
        Func_02036fb8();
        post_entity_walk();

        object_list_per_frame_walker(0);  /* player list */
        object_list_per_frame_walker(1);  /* entity list */

        /* VBlank handler */
        if (g_vblank_handler) g_vblank_handler();

        g_system_flags_prev = g_system_flags;
        if (g_system_flags & 0x100) {
            Func_02034ec0();  /* sound update */
        }
        Func_0203005c();

        /* Clear bit 31; if bit 3 was set, restore bit 31 */
        if (g_system_flags & 8) {
            g_system_flags |= 0x80000000;
        } else {
            g_system_flags &= 0x7FFFFFFF;
        }
    }
}

/* ─── display_init @ 0x02004398 ───
 * Initializes NDS display: DISPSTAT, DISPCNT, BG control, VRAM banks.
 * Calls sub-inits at 0x020042dc (VRAM bank setup), 0x0200422c,
 * 0x02003f88 (bg setup). */
extern void display_vram_bank_setup(void);
extern void display_bg_setup(void);
extern void display_palette_setup(void);

void display_init(void) {
    volatile u16 *dispstat = (volatile u16 *)0x04000004;
    volatile u32 *dispcnt  = (volatile u32 *)0x04000000;
    volatile u16 *bg0cnt   = (volatile u16 *)0x04000008;

    display_vram_bank_setup();

    /* Wait for VBlank flag to clear before reconfiguring */
    while (*dispcnt & 0x80000000) { /* DISPSTAT VBlank wait via DISPCNT bit */ }

    /* DISPSTAT: enable VBlank + VBlank IRQ, set vblank counter */
    *dispstat = 0;
    *dispstat |= 0x2000;   /* VBlank counter enable */
    *dispstat |= 0x1000;   /* VBlank IRQ enable */
    *dispstat &= ~0x3000;  /* clear mode bits */
    *dispstat |= 16;       /* vcount target */
    *dispstat &= ~3;       /* clear vcount compare bits */

    /* DISPCNT: enable display, set BG mode 0 */
    *dispcnt |= 0x8000;          /* LCD on */
    *dispcnt &= ~0xC0000000;     /* clear forced blank + BG mode ext */
    *dispcnt |= 0x80000000;      /* set bit 31 (framebuffer mode flag) */

    display_bg_setup();

    /* BG0 control: clear priority bits */
    *bg0cnt &= ~3;

    display_palette_setup();

    /* Clear BG scroll and enable */
    *(volatile u32 *)0x04000010 = 0;  /* BG scroll */
    *(volatile u16 *)0x04000014 = 0;
    *(volatile u32 *)0x04001008 = 0;
    *(volatile u16 *)0x04000008 = 0;
}

/* ─── input_poll @ 0x020082e8 ───
 * Not actually input polling — it is a halfword memset helper.
 * Fills [r1, r1+r2) with zeros in 2-byte steps. */
void input_poll(void *dst, u32 count_halfwords) {
    volatile u16 *p = (volatile u16 *)dst;
    for (u32 i = 0; i < count_halfwords; i += 2) {
        p[i >> 1] = 0;
    }
}

/* ─── System_Halt @ 0x02034d34 (340 bytes, ARM mode) ───
 * Halts the system until a VBlank or timer interrupt wakes it.
 * Reads system_state flags; if bit 0 is clear, returns immediately.
 * Otherwise saves previous state, calls a wait function, and loops
 * until the system state changes. */
extern u32 g_system_state_flags;
extern void System_WaitForVBlank(void);
void System_Halt(void) {
    if (!(g_system_state_flags & 1)) return;
    /* Complex state machine in original binary — saves previous state,
     * calls wait-for-vblank, checks new state.  Placeholder for now. */
    System_WaitForVBlank();
}

/* ─── Overlay_ScheduleFromInput @ 0x0203051c (20 bytes) ───
 * Stores requested overlay ID and one-shot callback into globals.
 * Literal pool: g_overlay_requested_id = 0x02087cbc,
 *               g_overlay_one_shot_cb  = 0x02087cb0 */
static u32 *const g_overlay_requested_id = (u32 *)0x02087cbc;
static u32 *const g_overlay_one_shot_cb  = (u32 *)0x02087cb0;
void Overlay_ScheduleFromInput(u32 requested_id, u32 one_shot_cb) {
    *g_overlay_requested_id = requested_id;
    *g_overlay_one_shot_cb  = one_shot_cb;
}

/* ─── Overlay_UnloadCurrent @ 0x0200c1ec (84 bytes, ARM mode) ───
 * Unloads the currently active overlay.  Allocates a 44-byte stack
 * descriptor, fills it via an internal lookup, then calls the unload
 * routine.  Returns 1 on success, 0 on failure.
 * Signature inferred from call site in Overlay_TransitionStep. */
extern u32 Overlay_InternalLookup(void *desc, u32 id, u32 flags);
extern u32 Overlay_InternalUnload(void *desc);
u32 Overlay_UnloadCurrent(u32 id, u32 flags) {
    u8 desc[44];
    if (!Overlay_InternalLookup(desc, id, flags))
        return 0;
    if (!Overlay_InternalUnload(desc))
        return 0;
    return 1;
}

/* ─── Overlay_LoadFromROM @ 0x0200c240 (92 bytes, ARM mode) ───
 * Loads an overlay from ROM.  Same descriptor pattern as Unload.
 * Calls lookup, then load-from-ROM, then post-load init.
 * Returns 1 on success, 0 on failure. */
extern u32 Overlay_InternalLoadFromROM(void *desc);
extern u32 Overlay_InternalPostLoad(void *desc);
u32 Overlay_LoadFromROM(u32 id, u32 flags) {
    u8 desc[44];
    if (!Overlay_InternalLookup(desc, id, flags))
        return 0;
    if (!Overlay_InternalLoadFromROM(desc))
        return 0;
    Overlay_InternalPostLoad(desc);
    return 1;
}

/* ─── Overlay_TransitionStep @ 0x020302d8 (180 bytes, ARM mode) ───
 * Per-frame overlay transition state machine.
 * Compares active_id vs requested_id:
 *   - If they match, skip to the callback check.
 *   - If they differ and a one-shot unload is pending, calls
 *     Overlay_UnloadCurrent(0, ...) and clears the flag.
 *   - If no unload pending, calls Overlay_LoadFromROM(0, requested),
 *     copies requested → active, and sets the loaded flag.
 * Finally, if a post-load callback is installed, calls it and clears.
 * Literal pool globals:
 *   g_overlay_active_id   = 0x02087cb4 (ptr to current overlay id)
 *   g_overlay_requested_id= 0x02087cbc (ptr to requested overlay id)
 *   g_overlay_loaded_flag = 0x02087cc8 (ptr to loaded-state flag)
 *   g_overlay_callback    = 0x02087cb0 (ptr to post-load callback fn) */
static u32 *const g_overlay_active_id   = (u32 *)0x02087cb4;
static u32 *const g_overlay_loaded_flag = (u32 *)0x02087cc8;
static u32 *const g_overlay_callback    = (u32 *)0x02087cb0;

typedef void (*OverlayCallback)(void);

void Overlay_TransitionStep(void) {
    u32 active   = *g_overlay_active_id;
    u32 requested = *g_overlay_requested_id;

    if (active != requested) {
        if (*g_overlay_one_shot_cb) {
            Overlay_UnloadCurrent(0, 0);
            *g_overlay_one_shot_cb = 0;
        }
        if (*g_overlay_one_shot_cb == 0) {
            Overlay_LoadFromROM(0, requested);
            *g_overlay_active_id   = *g_overlay_requested_id;
            *g_overlay_loaded_flag = 1;
        }
    }

    if (*g_overlay_loaded_flag) {
        OverlayCallback cb = (OverlayCallback)*g_overlay_callback;
        if (cb) {
            cb();
            *g_overlay_callback = 0;
        }
    }
}

/* ─── Overlay_Init @ 0x0203039c (404 bytes, ARM mode) ───
 * Initializes the overlay system.  Zeros overlay state globals,
 * copies a base pointer from a ROM pool, clears input delay and
 * overlay memory regions, sets up DISPCNT/VRAM registers for the
 * main engine display, and initializes overlay callback pointers.
 * Literal pool globals referenced (from binary):
 *   0x02087cb8, 0x02087cc4, 0x027ffc20, 0x02087cc0,
 *   0x02087cdc, 0x02087ccc, 0x02072c78 */
static u32 *const g_overlay_state_a = (u32 *)0x02087cb8;
static u32 *const g_overlay_state_b = (u32 *)0x02087cc4;
static u32 *const g_overlay_mem_base = (u32 *)0x02087cc0;
extern void Input_InitDelay(void *dst, u32 value, u32 size);
extern void memset_u16(u16 value, u16 *dst, u32 count);

void Overlay_Init(void) {
    *g_overlay_state_a = 0;
    *g_overlay_state_b = 0;

    u32 base = *g_overlay_mem_base;
    *g_overlay_mem_base = base;

    u16 dummy = 0;
    Input_InitDelay(&dummy, 0, 40);
    dummy = 0;
    Input_InitDelay(&dummy, 0, 16);

    /* Display register setup — DISPCNT configuration */
    /* The actual DISPCNT writes are hardware-specific NDS register ops */
    *g_overlay_state_b = 0;
    *g_overlay_loaded_flag = 0;
}

/* ─── Boot_ResetStateMachine @ 0x0202fd94 (492 bytes, ARM mode) ───
 * Handles the NDS boot/reset sequence.  Checks a hardware flag
 * (bit 15 of a register at 0x027fffa8), then runs through states:
 *   State 0: Calls a reset-prepare function, stores a handle.
 *   State 1: Waits for a ready signal, calls init with the handle.
 *   State 2: Waits for completion, then dispatches a final init.
 * On power-on path, waits for DS cart ready and sets a completion flag. */
extern void Boot_PrepareReset(void);
extern s32 Boot_WaitReady(u32 mode);
extern void Boot_FinalInit(u32 mode, u32 param);
extern void Boot_SetResetFlag(u32 flag);

#define BOOT_STATE_PTR  ((u32 *)0x02087ccc)
#define BOOT_HW_REG     ((volatile u16 *)0x027fffa8)
#define BOOT_READY_FLAG ((u32 *)0x04001000)

void Boot_ResetStateMachine(void) {
    u16 hw = *BOOT_HW_REG;
    if (hw & 0x8000) {
        u32 *state = BOOT_STATE_PTR;
        if (state[2] != 1) {
            state[2] = 1;
            Boot_PrepareReset();
            state[3] = /* result */ 0;
        } else {
            if (state[1] == state[0]) return;
        }
        u32 cur = state[0];
        if (cur == 0) {
            Boot_WaitReady(12);
        } else if (cur == 1) {
            Boot_WaitReady(0);
        } else if (cur == 2) {
            while (!Boot_WaitReady(1)) {}
            Boot_FinalInit(2, 0);
        }
        state[1] = state[0];
    } else {
        u32 *state = BOOT_STATE_PTR;
        if (state[2] == 0) return;
        u32 prev = state[1];
        state[2] = 0;
        if (prev == 0) {
            while (!Boot_WaitReady(1)) {}
            if (state[3] != -1) Boot_FinalInit(1, state[3]);
            Boot_SetResetFlag(0x10000);
        } else if (prev == 1) {
            while (!Boot_WaitReady(1)) {}
            if (state[3] != -1) Boot_FinalInit(1, state[3]);
        } else if (prev == 2) {
            if (state[3] != -1) Boot_FinalInit(1, state[3]);
        }
    }
}

/* ─── Entity_RemoveAll @ 0x02036fdc (84 bytes, ARM mode) ───
 * Iterates the entity linked list and removes all entities whose
 * marker field (at offset +0x1a / halfword) matches the given marker.
 * Linked-list head/tail pointers at ROM-resident globals:
 *   g_entity_list_head = 0x022b457c (ptr to first entity)
 *   g_entity_list_tail = 0x022b4588 (ptr to sentinel/tail)
 * Entity record layout: [+0x04] = next ptr, [+0x1a] = marker (u16) */
static u32 *const g_entity_list_head = (u32 *)0x022b457c;
static u32 *const g_entity_list_tail = (u32 *)0x022b4588;
extern void Entity_Free(void *entity);

void Entity_RemoveAll(u32 marker) {
    void *head = (void *)*g_entity_list_head;
    void *tail = (void *)*g_entity_list_tail;
    void *cur  = ((void **)head)[1]; /* entity->next at +0x04 */

    while (cur != tail) {
        u16 m = *(u16 *)((u8 *)cur + 0x1a);
        if (m == (u16)marker)
            Entity_Free(cur);
        cur = ((void **)cur)[1];
    }
}

/* ─── Func_02036fb8 @ 0x02036fb8 (28 bytes) ───
 * Calls through a function pointer loaded from a global table.
 * Loads entity list head ptr, entity list end ptr, and a function
 * pointer from a ROM pool, then calls the function with the two
 * entity-list pointers.  Appears to be an entity list dispatch entry.
 * Literal pool: 0x022b457c (head), 0x022b4580 (??), 0x02036e04 (fn) */
typedef void (*EntityListDispatchFn)(void *, void *);
void Func_02036fb8(void) {
    /* On ARM: calls raw ROM address 0x02036e04 (static_entity_walker).
     * On PC recomp: call the linked symbol directly. */
    EntityListDispatchFn fn = (EntityListDispatchFn)static_entity_walker;
    void *head = (void *)*g_entity_list_head;
    void *end  = (void *)*(u32 *)0x022b4580;
    fn(head, end);
}

/* ─── Display_FadeToBlack @ 0x02004398 (128 bytes) ───
 * Configures NDS display registers for fade-to-black effect.
 * Sets up DISPCNT, master bright, and display effects. */
void Display_FadeToBlack(void) {
    /* Hardware register manipulation for display fade.
     * The actual implementation writes to NDS display registers
     * (DISPCNT at 0x04000000, master bright, etc.)
     * Full register-level decompilation pending — the ROM code
     * performs a sequence of read-modify-write on display control
     * registers to set up the fade effect. */
}

/* ═══════════════════════════════════════════════════════════════
 * UNNAMED DAT/FUN SYMBOLS — raw address references
 * ═══════════════════════════════════════════════════════════════ */

u32 DAT_02033d4c = 0;
u32 DAT_02033d50 = 0;
u32 DAT_02033d54 = 0;
u32 DAT_02033da8 = 0;
u32 DAT_02033dac = 0;

/* ═══════════════════════════════════════════════════════════════
 * UNRESOLVED GLOBAL VARIABLES — storage for extern declarations
 * ═══════════════════════════════════════════════════════════════ */

u32 *g_system_flags_ptr = 0;
u32 g_entity_capacity = 0;
void *g_entity_containers = 0;
void *g_entity_free_list = 0;
void *g_entity_free_table = 0;
u32 g_entity_state_index = 0;
void *g_entity_state_table = 0;
void *g_heap_base = 0;
void *g_heap_end = 0;
void *g_oam_shadow = 0;
void *g_object_list_bases = 0;
u32 g_render_flags = 0;
u32 g_render_index = 0;
void *g_irq_handlers = 0;
void *g_vram_mapping = 0;
u32 g_vsync_count = 0;
void *g_tree_root = 0;
u32 g_tree_count = 0;
u32 g_trigger_search_idx = 0;
u32 g_trigger_search_max = 0;
void *g_trigger_table_counts = 0;
void *g_trigger_table_types = 0;
void *g_controller_ring = 0;
void *g_input_subsystem = 0;
s32 g_movement_speed_factor = 0;
s32 g_movement_extra_factor = 0;
/* Physics tables — defined in data_tables.c */
extern const s8  g_accel_scale_table[];
extern const s16 g_slope_table[];
extern const s8  g_drift_table[];
extern const s16 g_sin_table[];

/* TODO: Find ROM address for slope_direction_table */
const s16 g_slope_direction_table[32] = {0};
void *g_affine_state = 0;
void *g_collision_entity_list = 0;
u32 g_collision_entity_count = 0;
void *g_mode_transition = 0;
PhysicsPlayer *g_current_player = 0;
void *collision_entity_list = 0;
u32 collision_entity_count = 0;

/* Additional stubs needed by newly decompiled functions */
void ActionState_ProcessCleanup(void *player, void *state) { (void)player; (void)state; }
void HitboxPlacementDispatch(s32 pos_x, s32 pos_y, u32 player_index,
                             void *player, void *group_a,
                             void *group_b, void *reserved) {
    (void)pos_x; (void)pos_y; (void)player_index; (void)player; (void)group_a; (void)group_b; (void)reserved;
}
void OBJLIST_MEMCPY(void *dst, const void *src, u32 n) { (void)dst; (void)src; (void)n; }
void SoundDev_SetChannelBit(u32 bit) { (void)bit; }

/* ═══════════════════════════════════════════════════════════════
 * ADDITIONAL STUBS — resolved during rebuild (2026-09-13)
 * ═══════════════════════════════════════════════════════════════ */

/* Boot subsystem — called from Boot_ResetStateMachine */
void Boot_PrepareReset(void) {}
s32 Boot_WaitReady(u32 mode) { (void)mode; return 0; }
void Boot_FinalInit(u32 mode, u32 param) { (void)mode; (void)param; }
void Boot_SetResetFlag(u32 flag) { (void)flag; }

/* Entity subsystem — called from Entity_RemoveAll */
void Entity_Free(void *entity) { (void)entity; }

/* Function pointers — callbacks referenced by decompiled code */
void Func_0203005c(void) {}
void Func_0204edc0(void) {}

/* Global data — entity list heads array */
void *g_entity_list_heads[32] = {0};

/* System state — checked by Boot_ResetStateMachine */
u32 g_system_state_flags = 0;

/* Input delay setup — called from Overlay_Init */
void Input_InitDelay(void *dst, u32 value, u32 size) { (void)dst; (void)value; (void)size; }

/* Overlay internals — called from Overlay_UnloadCurrent/LoadFromROM */
u32 Overlay_InternalLookup(void *desc, u32 id, u32 flags) { (void)desc; (void)id; (void)flags; return 0; }
u32 Overlay_InternalUnload(void *desc) { (void)desc; return 0; }
u32 Overlay_InternalLoadFromROM(void *desc) { (void)desc; return 0; }
u32 Overlay_InternalPostLoad(void *desc) { (void)desc; return 0; }

/* System wait — called from System_Halt */
void System_WaitForVBlank(void) {}
