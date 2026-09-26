/* Gameplay level ring helper functions. */

#include "nds_types.h"

void RingHelper_Init(void) {
    g_ringhelper_count = 0;
}

s32 RingHelper_Add(s32 x, s32 y, u32 param) {
    if (g_ringhelper_count >= 64) return -1;
    u32 i = g_ringhelper_count;
    g_ringlayouts[i].tile_id = x;
    g_ringlayouts[i].num_frames = y;
    g_ringlayouts[i].speed = param;
    g_ringlayouts[i].timer = 0;
    g_ringlayouts[i].frame = 0;
    g_ringlayouts[i].active = 1;
    g_ringhelper_count++;
    return i;
}

void RingHelper_Update(void) {
    u32 i;
    for (i = 0; i < g_ringhelper_count; i++) {
        if (!g_ringlayouts[i].active) continue;
        g_ringlayouts[i].timer++;
        if (g_ringlayouts[i].timer >= g_ringlayouts[i].speed) {
            g_ringlayouts[i].timer = 0;
            g_ringlayouts[i].frame++;
            if (g_ringlayouts[i].frame >= g_ringlayouts[i].num_frames) {
                g_ringlayouts[i].frame = 0;
            }
        }
    }
}

void RingHelper_Remove(u32 index) {
    if (index < g_ringhelper_count) g_ringlayouts[index].active = 0;
}

u32 RingHelper_GetCount(void) { return g_ringhelper_count; }

void RingHelper_Clear(void) { g_ringhelper_count = 0; }
void RingHelper_Reset(void) { g_ringhelper_count = 0; }
