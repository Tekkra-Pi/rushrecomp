/* Gameplay level boss shield functions. */

#include "nds_types.h"

static u32 s_bossshield_active;
static u32 s_bossshield_hp;
static u32 s_bossshield_timer;

void BossShield_Init(void) {
    s_bossshield_active = 0;
    s_bossshield_hp = 0;
    s_bossshield_timer = 0;
    g_bossshield_active = 0;
    g_bossshield_hp = 0;
}

void BossShield_Start(void) {
    s_bossshield_active = 1;
    s_bossshield_hp = 3;
    s_bossshield_timer = 0;
    g_bossshield_active = 1;
    g_bossshield_hp = 3;
}

void BossShield_Stop(void) {
    s_bossshield_active = 0;
    g_bossshield_active = 0;
}

void BossShield_Update(void) {
    if (!s_bossshield_active) return;
    s_bossshield_timer++;
}

s32 BossShield_Hit(u32 index) {
    (void)index;
    if (!s_bossshield_active) return 0;
    if (s_bossshield_hp > 0) {
        s_bossshield_hp--;
        g_bossshield_hp = s_bossshield_hp;
        if (s_bossshield_hp <= 0) {
            BossShield_Stop();
            return 1;
        }
    }
    return 0;
}

s32 BossShield_IsActive(u32 index) {
    (void)index;
    return s_bossshield_active;
}

s32 BossShield_GetHP(u32 index) {
    (void)index;
    return s_bossshield_hp;
}

void BossShield_Reset(void) {
    s_bossshield_active = 0;
    s_bossshield_hp = 0;
    s_bossshield_timer = 0;
    g_bossshield_active = 0;
    g_bossshield_hp = 0;
}
