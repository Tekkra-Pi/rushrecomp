/* Gameplay explosion effect functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"


/* Explosion_Init @ 0x02026400 (168 bytes)
 * Initializes explosion effect system. */
void Explosion_Init(void) {
    g_explosion_count = 0;
}

/* Explosion_Spawn @ 0x020264a8 (232 bytes)
 * Spawns explosion effect.
 * Args: r0=x, r1=y, r2=size
 * Returns: index or -1 */
s32 Explosion_Spawn(s32 x, s32 y, u32 size) {
    if (g_explosion_count >= 8) return -1;
    u32 i = g_explosion_count;
    g_explosions[i].x = x;
    g_explosions[i].y = y;
    g_explosions[i].size = size;
    g_explosions[i].frame = 0;
    g_explosions[i].timer = 0;
    g_explosions[i].active = 1;
    g_explosion_count++;
    SoundEffect_Play(0x94, 0x40, 0x100);
    CameraShake_Start(size * 2, 15);
    return i;
}

/* Explosion_Update @ 0x02026590 (296 bytes)
 * Updates explosion effects. */
void Explosion_Update(void) {
    u32 i;
    for (i = 0; i < g_explosion_count; i++) {
        if (g_explosions[i].active) {
            g_explosions[i].timer++;
            /* Advance frame every 3 ticks */
            if (g_explosions[i].timer % 3 == 0) {
                g_explosions[i].frame++;
            }
            /* Explosion animation is 6 frames */
            if (g_explosions[i].frame >= 6) {
                g_explosions[i].active = 0;
            }
        }
    }
}

/* Explosion_Draw @ 0x020266b8
 * Draws explosion effects. */
void Explosion_Draw(void) {
    u32 i;
    for (i = 0; i < g_explosion_count; i++) {
        if (g_explosions[i].active) {
            u32 tile = 0x260 + g_explosions[i].frame;
            u32 size_attr = g_explosions[i].size ? 0x2000 : 0x1000;
            s32 ox = g_explosions[i].size ? -16 : -8;
            s32 oy = g_explosions[i].size ? -16 : -8;
            OAM_AddSprite(g_explosions[i].x >> 8 + ox,
                          g_explosions[i].y >> 8 + oy,
                          tile, size_attr);
        }
    }
}

/* Explosion_Remove @ 0x02026760
 * Removes explosion effect.
 * Args: r0=index */
void Explosion_Remove(u32 index) {
    if (index < g_explosion_count) {
        g_explosions[index].active = 0;
    }
}

/* Explosion_GetCount @ 0x020267a0
 * Returns active explosion count.
 * Returns: count */
u32 Explosion_GetCount(void) {
    u32 count = 0;
    u32 i;
    for (i = 0; i < g_explosion_count; i++) {
        if (g_explosions[i].active) count++;
    }
    return count;
}

/* Explosion_IsActive @ 0x020267e0
 * Returns whether explosion is active.
 * Args: r0=index
 * Returns: 1 if active, 0 otherwise */
s32 Explosion_IsActive(u32 index) {
    if (index < g_explosion_count) {
        return g_explosions[index].active;
    }
    return 0;
}

/* Explosion_GetSize @ 0x02026820
 * Returns explosion size.
 * Args: r0=index
 * Returns: size */
u32 Explosion_GetSize(u32 index) {
    if (index < g_explosion_count) {
        return g_explosions[index].size;
    }
    return 0;
}

/* Explosion_GetFrame @ 0x02026860
 * Returns explosion frame.
 * Args: r0=index
 * Returns: frame */
u32 Explosion_GetFrame(u32 index) {
    if (index < g_explosion_count) {
        return g_explosions[index].frame;
    }
    return 0;
}

/* Explosion_Clear @ 0x020268a0
 * Clears all explosions. */
void Explosion_Clear(void) {
    g_explosion_count = 0;
}
