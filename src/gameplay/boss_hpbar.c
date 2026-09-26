/* Boss HP bar functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_bosshpbar_state;
static u32 s_bosshpbar_timer;
static u32 s_bosshpbar_active;
static u32 s_bosshpbar_hp;

void BossHpBar_Init(void) {
    s_bosshpbar_state = 0;
    s_bosshpbar_timer = 0;
    s_bosshpbar_active = 0;
    s_bosshpbar_hp = 0;
    g_bosshpbar_active = 0;
}

void BossHpBar_Start(void) {
    s_bosshpbar_state = 1;
    s_bosshpbar_timer = 0;
    s_bosshpbar_active = 1;
    g_bosshpbar_active = 1;
}

void BossHpBar_Update(void) {
    if (!s_bosshpbar_active) return;
    s_bosshpbar_timer++;
    u32 i;
    for (i = 0; i < g_bossactor_count; i++) {
        if (!g_bossactors[i].active) continue;
        s_bosshpbar_hp = g_bossactors[i].hp;
        break;
    }
}

void BossHpBar_Draw(void) {
    if (!s_bosshpbar_active) return;
}

void BossHpBar_Stop(void) {
    s_bosshpbar_state = 0;
    s_bosshpbar_active = 0;
    g_bosshpbar_active = 0;
}

s32 BossHpBar_IsActive(u32 index) {
    (void)index;
    return s_bosshpbar_active;
}

s32 BossHpBar_GetHP(u32 index) {
    (void)index;
    return s_bosshpbar_hp;
}

void BossHpBar_Reset(void) {
    s_bosshpbar_state = 0;
    s_bosshpbar_timer = 0;
    s_bosshpbar_active = 0;
    s_bosshpbar_hp = 0;
    g_bosshpbar_active = 0;
}
