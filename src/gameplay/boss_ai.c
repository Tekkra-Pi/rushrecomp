/* Gameplay level boss AI functions. */
#include "nds_types.h"


void BossAI_Init(void) { g_bossai_count = 0; }

s32 BossAI_Add(u32 boss_idx, u32 ai_type) {
    if (g_bossai_count >= 4) return -1;
    u32 i = g_bossai_count;
    g_bossais[i].boss_idx = boss_idx; g_bossais[i].ai_type = ai_type;
    g_bossais[i].timer = 0; g_bossais[i].phase = 0;
    g_bossais[i].active = 1;
    g_bossai_count++; return i;
}

void BossAI_Update(void) {
    u32 i;
    for (i = 0; i < g_bossai_count; i++) {
        if (!g_bossais[i].active) continue;
        g_bossais[i].timer++;
        u32 bi = g_bossais[i].boss_idx;
        if (bi >= g_bossactor_count || !g_bossactors[bi].active) continue;
        switch (g_bossais[i].ai_type) {
            case 0: /* move back and forth */
                g_bossactors[bi].vx = Math_Sin(g_bossais[i].timer * 4) * 0x40;
                break;
            case 1: /* chase player */
            {
                s32 px, py;
                Player_GetPosition(&px, &py);
                s32 dx = px - g_bossactors[bi].x;
                g_bossactors[bi].vx = (dx > 0) ? 0x40 : -0x40;
                break;
            }
            case 2: /* jump and stomp */
                if (g_bossais[i].timer % 90 == 0) {
                    g_bossactors[bi].vy = -0x300;
                }
                break;
        }
    }
}

void BossAI_Remove(u32 i) { if (i < g_bossai_count) g_bossai_count--; }
u32 BossAI_GetCount(void) { return g_bossai_count; }
void BossAI_Clear(void) { g_bossai_count = 0; }
void BossAI_Reset(void) { g_bossai_count = 0; }
