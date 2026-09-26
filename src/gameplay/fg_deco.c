/* Gameplay level foreground decoration functions. */
#include "nds_types.h"


void FgDeco_Init(void) { g_fgdeco_count = 0; }

s32 FgDeco_Add(s32 x, s32 y, u32 tile_id, u32 flags) {
    if (g_fgdeco_count >= 32) return -1;
    u32 i = g_fgdeco_count;
    g_fgdecos[i].x = x; g_fgdecos[i].y = y; g_fgdecos[i].tile_id = tile_id;
    g_fgdecos[i].flags = flags; g_fgdecos[i].active = 1;
    g_fgdeco_count++; return i;
}

void FgDeco_Remove(u32 i) { if (i < g_fgdeco_count) g_fgdecos[i].active = 0; }

void FgDeco_Draw(void) {
    u32 i;
    for (i = 0; i < g_fgdeco_count; i++) {
        if (g_fgdecos[i].active) {
            s32 sx = g_fgdecos[i].x >> 8;
            s32 sy = g_fgdecos[i].y >> 8;
            if (sx > -32 && sx < 288 && sy > -32 && sy < 224) {
                OAM_AddSprite(sx, sy, g_fgdecos[i].tile_id, g_fgdecos[i].flags);
            }
        }
    }
}

u32 FgDeco_GetCount(void) { return g_fgdeco_count; }
s32 FgDeco_IsActive(u32 i) { return (i < g_fgdeco_count) ? g_fgdecos[i].active : 0; }
void FgDeco_Clear(void) { g_fgdeco_count = 0; }
void FgDeco_Reset(void) { g_fgdeco_count = 0; }
