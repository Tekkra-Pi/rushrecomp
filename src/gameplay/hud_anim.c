/* Gameplay HUD animation functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"


/* HUDAnim_Init @ 0x02018800 (192 bytes)
 * Initializes HUD animation system. */
void HUDAnim_Init(void) {
    g_hud_anim_count = 0;
}

/* HUDAnim_Add @ 0x020188c0 (288 bytes)
 * Adds animation to HUD.
 * Args: r0=sprite_id, r1=x, r2=y, r3=num_frames
 * Returns: anim index or -1 */
s32 HUDAnim_Add(u32 sprite_id, s32 x, s32 y, u32 num_frames) {
    if (g_hud_anim_count >= 16) return -1;
    u32 i = g_hud_anim_count;
    g_hud_anims[i].sprite_id = sprite_id;
    g_hud_anims[i].x = x;
    g_hud_anims[i].y = y;
    g_hud_anims[i].num_frames = num_frames;
    g_hud_anims[i].current_frame = 0;
    g_hud_anims[i].timer = 0;
    g_hud_anims[i].speed = 4;
    g_hud_anims[i].loop = 1;
    g_hud_anims[i].active = 1;
    g_hud_anim_count++;
    return i;
}

/* HUDAnim_Remove @ 0x020189e0 (312 bytes)
 * Removes animation from HUD.
 * Args: r0=anim_index */
void HUDAnim_Remove(u32 anim_index) {
    if (anim_index < g_hud_anim_count) {
        g_hud_anims[anim_index].active = 0;
    }
}

/* HUDAnim_Update @ 0x02018b18
 * Updates all HUD animations. */
void HUDAnim_Update(void) {
    u32 i;
    for (i = 0; i < g_hud_anim_count; i++) {
        if (g_hud_anims[i].active) {
            g_hud_anims[i].timer++;
            if (g_hud_anims[i].timer >= g_hud_anims[i].speed) {
                g_hud_anims[i].timer = 0;
                g_hud_anims[i].current_frame++;
                if (g_hud_anims[i].current_frame >= g_hud_anims[i].num_frames) {
                    if (g_hud_anims[i].loop) {
                        g_hud_anims[i].current_frame = 0;
                    } else {
                        g_hud_anims[i].active = 0;
                    }
                }
            }
        }
    }
}

/* HUDAnim_Draw @ 0x02018be0
 * Draws all HUD animations. */
void HUDAnim_Draw(void) {
    u32 i;
    for (i = 0; i < g_hud_anim_count; i++) {
        if (g_hud_anims[i].active) {
            OAM_AddSprite(g_hud_anims[i].x, g_hud_anims[i].y,
                          g_hud_anims[i].sprite_id, g_hud_anims[i].current_frame);
        }
    }
}

/* HUDAnim_SetSpeed @ 0x02018c40
 * Sets animation speed.
 * Args: r0=anim_index, r1=speed */
void HUDAnim_SetSpeed(u32 anim_index, u32 speed) {
    if (anim_index < g_hud_anim_count) {
        g_hud_anims[anim_index].speed = speed;
    }
}

/* HUDAnim_SetLoop @ 0x02018c80
 * Sets animation loop flag.
 * Args: r0=anim_index, r1=loop */
void HUDAnim_SetLoop(u32 anim_index, u32 loop) {
    if (anim_index < g_hud_anim_count) {
        g_hud_anims[anim_index].loop = loop;
    }
}

/* HUDAnim_Play @ 0x02018cc0
 * Plays animation.
 * Args: r0=anim_index */
void HUDAnim_Play(u32 anim_index) {
    if (anim_index < g_hud_anim_count) {
        g_hud_anims[anim_index].active = 1;
        g_hud_anims[anim_index].current_frame = 0;
        g_hud_anims[anim_index].timer = 0;
    }
}

/* HUDAnim_Stop @ 0x02018d10
 * Stops animation.
 * Args: r0=anim_index */
void HUDAnim_Stop(u32 anim_index) {
    if (anim_index < g_hud_anim_count) {
        g_hud_anims[anim_index].active = 0;
    }
}

/* HUDAnim_IsComplete @ 0x02018d50
 * Returns whether animation is complete.
 * Args: r0=anim_index
 * Returns: 1 if complete, 0 otherwise */
s32 HUDAnim_IsComplete(u32 anim_index) {
    if (anim_index < g_hud_anim_count) {
        return !g_hud_anims[anim_index].active;
    }
    return 1;
}

/* HUDAnim_Clear @ 0x02018d90 (296 bytes)
 * Clears all HUD animations. */
void HUDAnim_Clear(void) {
    g_hud_anim_count = 0;
}
