/* Gameplay level debris functions. */
#include "nds_types.h"


void Debris_Init(void) { g_debris_count = 0; }

s32 Debris_Spawn(s32 x, s32 y, u32 type) {
    if (g_debris_count >= 32) return -1;
    u32 i = g_debris_count;
    g_debris[i].x = x; g_debris[i].y = y;
    g_debris[i].type = type; g_debris[i].vx = (Math_Rand() & 0x3ff) - 0x200;
    g_debris[i].vy = -(Math_Rand() & 0x3ff);
    g_debris[i].timer = 0; g_debris[i].active = 1;
    g_debris_count++; return i;
}

void Debris_Update(void) {
    u32 i;
    for (i = 0; i < g_debris_count; i++) {
        if (!g_debris[i].active) continue;
        g_debris[i].vy += 0x100;
        g_debris[i].x += g_debris[i].vx;
        g_debris[i].y += g_debris[i].vy;
        g_debris[i].timer++;
        if (g_debris[i].timer >= 60) g_debris[i].active = 0;
    }
}

void Debris_Draw(void) {
    u32 i;
    for (i = 0; i < g_debris_count; i++) {
        if (g_debris[i].active) {
            OAM_AddSprite(g_debris[i].x >> 8, g_debris[i].y >> 8, 0x380 + g_debris[i].type, 0x1000);
        }
    }
}

void Debris_Remove(u32 i) { if (i < g_debris_count) g_debris_count--; }
u32 Debris_GetCount(void) { return g_debris_count; }
void Debris_Clear(void) { g_debris_count = 0; }
void Debris_Reset(void) { g_debris_count = 0; }
