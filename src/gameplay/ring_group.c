/* Gameplay level ring group functions. */
#include "nds_types.h"


void RingGroup_Init(void) { g_ringgroup_count = 0; }

s32 RingGroup_Add(s32 x, s32 y, u32 count, u32 spacing) {
    if (g_ringgroup_count >= 16) return -1;
    u32 i = g_ringgroup_count;
    g_ringgroups[i].x = x; g_ringgroups[i].y = y;
    g_ringgroups[i].count = count; g_ringgroups[i].spacing = spacing;
    g_ringgroups[i].collected = 0; g_ringgroups[i].active = 1;
    g_ringgroup_count++; return i;
}

void RingGroup_Update(void) {
    u32 i;
    for (i = 0; i < g_ringgroup_count; i++) {
        if (!g_ringgroups[i].active || g_ringgroups[i].collected) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        u32 j;
        for (j = 0; j < g_ringgroups[i].count; j++) {
            s32 rx = g_ringgroups[i].x + j * g_ringgroups[i].spacing;
            s32 dx = px - rx;
            s32 dy = py - g_ringgroups[i].y;
            if (dx >= -0x600 && dx <= 0x600 && dy >= -0x600 && dy <= 0x600) {
                Player_AddRings(1);
            }
        }
        s32 dx_end = px - (g_ringgroups[i].x + g_ringgroups[i].count * g_ringgroups[i].spacing);
        if (dx_end < -g_ringgroups[i].spacing || dx_end > g_ringgroups[i].spacing) {
            g_ringgroups[i].collected = 1;
        }
    }
}

void RingGroup_Remove(u32 i) { if (i < g_ringgroup_count) g_ringgroup_count--; }
u32 RingGroup_GetCount(void) { return g_ringgroup_count; }
void RingGroup_Clear(void) { g_ringgroup_count = 0; }
void RingGroup_Reset(void) { g_ringgroup_count = 0; }
