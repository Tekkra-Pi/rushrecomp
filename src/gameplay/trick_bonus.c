/* Gameplay level trick bonus functions. */
#include "nds_types.h"

void TrickBonus_Init(void) { g_trickbonus_count = 0; }

void TrickBonus_Add(u32 trick_type, s32 x, s32 y) {
    if (g_trickbonus_count >= 4) return;
    u32 i = g_trickbonus_count;
    g_trickbonus[i].trick_type = trick_type;
    g_trickbonus[i].x = x; g_trickbonus[i].y = y;
    g_trickbonus[i].timer = 0; g_trickbonus[i].active = 1;
    g_trickbonus_count++;
}

void TrickBonus_Update(void) {
    u32 i;
    for (i = 0; i < g_trickbonus_count; i++) {
        if (!g_trickbonus[i].active) continue;
        g_trickbonus[i].timer++;
        g_trickbonus[i].y -= 0x80;
        if (g_trickbonus[i].timer >= 60) g_trickbonus[i].active = 0;
    }
}

void TrickBonus_Remove(u32 i) { if (i < g_trickbonus_count) g_trickbonus_count--; }
u32 TrickBonus_GetCount(void) { return g_trickbonus_count; }
void TrickBonus_Clear(void) { g_trickbonus_count = 0; }
void TrickBonus_Reset(void) { g_trickbonus_count = 0; }
