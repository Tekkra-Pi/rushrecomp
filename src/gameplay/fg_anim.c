/* Gameplay level fg anim functions. */
#include "nds_types.h"

#define FGANIM_MAX 16

static u32 s_fganim_count;

void FgAnim_Init(void) { s_fganim_count = 0; }

s32 FgAnim_Add(s32 x, s32 y, u32 param) {
    if (s_fganim_count >= FGANIM_MAX) return -1;
    u32 i = s_fganim_count;
    g_fganims[i].tile_id = param;
    g_fganims[i].num_frames = 0;
    g_fganims[i].speed = 1;
    g_fganims[i].timer = 0;
    g_fganims[i].frame = 0;
    g_fganims[i].active = 1;
    s_fganim_count++;
    return i;
}

void FgAnim_Update(void) {
    u32 i;
    for (i = 0; i < s_fganim_count; i++) {
        if (!g_fganims[i].active) continue;
        g_fganims[i].timer++;
        if (g_fganims[i].timer >= g_fganims[i].speed) {
            g_fganims[i].timer = 0;
            g_fganims[i].frame++;
            if (g_fganims[i].frame >= g_fganims[i].num_frames) {
                g_fganims[i].frame = 0;
            }
        }
    }
}

void FgAnim_Remove(u32 index) {
    if (index < s_fganim_count) g_fganims[index].active = 0;
}

u32 FgAnim_GetFrame(u32 index) {
    if (index >= s_fganim_count) return 0;
    return g_fganims[index].frame;
}

u32 FgAnim_GetCount(void) { return s_fganim_count; }

s32 FgAnim_IsActive(u32 index) {
    if (index >= s_fganim_count) return 0;
    return g_fganims[index].active;
}

void FgAnim_Clear(void) { s_fganim_count = 0; }
void FgAnim_Reset(void) { s_fganim_count = 0; }
