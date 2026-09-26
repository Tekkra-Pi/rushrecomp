/* Gameplay level explosion FX functions. */
#include "nds_types.h"


void ExplosionFX_Init(void) { g_explosionfx_count = 0; }

s32 ExplosionFX_Spawn(s32 x, s32 y) {
    if (g_explosionfx_count >= 8) return -1;
    u32 i = g_explosionfx_count;
    g_explosionfxs[i].x = x; g_explosionfxs[i].y = y;
    g_explosionfxs[i].timer = 0; g_explosionfxs[i].frame = 0;
    g_explosionfxs[i].active = 1;
    g_explosionfx_count++; return i;
}

void ExplosionFX_Update(void) {
    u32 i;
    for (i = 0; i < g_explosionfx_count; i++) {
        if (!g_explosionfxs[i].active) continue;
        g_explosionfxs[i].timer++;
        g_explosionfxs[i].frame = g_explosionfxs[i].timer / 4;
        if (g_explosionfxs[i].timer >= 24) g_explosionfxs[i].active = 0;
    }
}

void ExplosionFX_Draw(void) {
    u32 i;
    for (i = 0; i < g_explosionfx_count; i++) {
        if (g_explosionfxs[i].active) {
            u32 tile = 0x3e0 + g_explosionfxs[i].frame * 4;
            OAM_AddSprite(g_explosionfxs[i].x >> 8, g_explosionfxs[i].y >> 8, tile, 0x1000);
        }
    }
}

void ExplosionFX_Remove(u32 i) { if (i < g_explosionfx_count) g_explosionfxs[i].active = 0; }
u32 ExplosionFX_GetCount(void) { return g_explosionfx_count; }
void ExplosionFX_Clear(void) { g_explosionfx_count = 0; }
void ExplosionFX_Reset(void) { g_explosionfx_count = 0; }
