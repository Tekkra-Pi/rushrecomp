/* Boss system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);
extern void Player_SetPosition(s32 x, s32 y);
extern void Player_AddScore(u32 score);

#define BOSS_MAX 4

void Boss_Update(void) {
    u32 i;
    for (i = 0; i < g_bossactor_count; i++) {
        if (!g_bossactors[i].active) continue;
        g_bossactors[i].x += g_bossactors[i].vx;
        g_bossactors[i].y += g_bossactors[i].vy;
        g_bossactors[i].timer++;
    }
}

void Boss_Destroy(void) {
    u32 i;
    for (i = 0; i < g_bossactor_count; i++) {
        if (!g_bossactors[i].active) continue;
        g_bossactors[i].active = 0;
        ExplosionFX_Spawn(g_bossactors[i].x, g_bossactors[i].y);
    }
}

u32 Boss_GetMaxHP(u32 index) {
    if (index >= g_bossactor_count) return 0;
    return g_bossactors[index].max_hp;
}

void Boss_Damage(void) {
    u32 i;
    for (i = 0; i < g_bossactor_count; i++) {
        if (!g_bossactors[i].active) continue;
        if (g_bossactors[i].hp > 0) {
            g_bossactors[i].hp--;
            if (g_bossactors[i].hp == 0) {
                Boss_Destroy();
                Player_AddScore(1000);
            }
        }
    }
}
