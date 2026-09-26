/* Gameplay level capsule functions. */
#include "nds_types.h"


void Capsule_Init(void) { g_capsule_count = 0; }

s32 Capsule_Add(s32 x, s32 y, u32 type) {
    if (g_capsule_count >= 4) return -1;
    u32 i = g_capsule_count;
    g_capsules[i].x = x; g_capsules[i].y = y;
    g_capsules[i].type = type; g_capsule_count++;
    g_capsules[i].active = 1;
    return i;
}

void Capsule_Update(void) {
    u32 i;
    for (i = 0; i < g_capsule_count; i++) {
        if (!g_capsules[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_capsules[i].x;
        s32 dy = py - g_capsules[i].y;
        if (dx >= -0x800 && dx <= 0x800 && dy >= -0x800 && dy <= 0x800) {
            g_capsules[i].active = 0;
            PowerupHelper_Give(g_capsules[i].type);
        }
    }
}

void Capsule_Remove(u32 i) { if (i < g_capsule_count) g_capsule_count--; }
u32 Capsule_GetCount(void) { return g_capsule_count; }
void Capsule_Clear(void) { g_capsule_count = 0; }
void Capsule_Reset(void) { g_capsule_count = 0; }
