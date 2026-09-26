/* Gameplay level chain bonus functions. */
#include "nds_types.h"


void ChainBonus_Init(void) { g_chainbonus_count = 0; }

void ChainBonus_Add(u32 bonus) {
    if (g_chainbonus_count >= 16) return;
    u32 i = g_chainbonus_count;
    g_chainbonus[i].bonus = bonus * (g_chainbonus_count + 1);
    g_chainbonus[i].timer = 0; g_chainbonus[i].active = 1;
    g_chainbonus_count++;
}

void ChainBonus_Update(void) {
    u32 i;
    for (i = 0; i < g_chainbonus_count; i++) {
        if (!g_chainbonus[i].active) continue;
        g_chainbonus[i].timer++;
        if (g_chainbonus[i].timer >= 60) {
            Player_AddScore(g_chainbonus[i].bonus);
            g_chainbonus[i].active = 0;
        }
    }
}

void ChainBonus_Remove(u32 i) { if (i < g_chainbonus_count) g_chainbonus_count--; }
u32 ChainBonus_GetCount(void) { return g_chainbonus_count; }
void ChainBonus_Clear(void) { g_chainbonus_count = 0; }
void ChainBonus_Reset(void) { g_chainbonus_count = 0; }
