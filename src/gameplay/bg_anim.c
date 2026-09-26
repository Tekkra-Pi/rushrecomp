/* Gameplay level background animation functions. */

#include "nds_types.h"

#define BGANIM_MAX 16

static u32 s_bganim_count;

void BgAnim_Init(void) { s_bganim_count = 0; }

s32 BgAnim_Add(s32 x, s32 y, u32 param) {
    if (s_bganim_count >= BGANIM_MAX) return -1;
    u32 i = s_bganim_count;
    g_bganims[i].bg_id = param;
    g_bganims[i].tile_count = 0;
    g_bganims[i].speed = 1;
    g_bganims[i].timer = 0;
    g_bganims[i].frame = 0;
    g_bganims[i].active = 1;
    s_bganim_count++;
    return i;
}

void BgAnim_Update(void) {
    u32 i;
    for (i = 0; i < s_bganim_count; i++) {
        if (!g_bganims[i].active) continue;
        g_bganims[i].timer++;
        if (g_bganims[i].timer >= g_bganims[i].speed) {
            g_bganims[i].timer = 0;
            g_bganims[i].frame++;
            if (g_bganims[i].frame >= g_bganims[i].tile_count) {
                g_bganims[i].frame = 0;
            }
        }
    }
}

void BgAnim_Remove(u32 index) {
    if (index < s_bganim_count) g_bganims[index].active = 0;
}

u32 BgAnim_GetFrame(u32 index) {
    if (index >= s_bganim_count) return 0;
    return g_bganims[index].frame;
}

u32 BgAnim_GetCount(void) { return s_bganim_count; }

s32 BgAnim_IsActive(u32 index) {
    if (index >= s_bganim_count) return 0;
    return g_bganims[index].active;
}

void BgAnim_Clear(void) { s_bganim_count = 0; }
void BgAnim_Reset(void) { s_bganim_count = 0; }
