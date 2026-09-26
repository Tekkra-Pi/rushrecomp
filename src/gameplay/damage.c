/* Damage and invincibility functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);
extern void Player_SetState(u32 state);
extern void Player_AddScore(u32 score);
extern void Sound_Play(u32 sfx);

s32 Damage_Check(void) {
    if (g_invinv_timer > 0) return 0;
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemies[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_enemies[i].x;
        s32 dy = py - g_enemies[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x300 && dy <= 0x300) {
            return 1;
        }
    }
    return 0;
}

void Damage_Apply(void) {
    g_player_state = 2;
    g_invinv_timer = 120;
    g_invinv_flicker = 1;
    g_hurtflash_timer = 20;
    g_hurtflash_alpha = 16;
    Sound_Play(0x10);
    if (g_game_rings > 0) {
        g_game_rings = 0;
    } else {
        g_lives--;
    }
}

void Damage_InvulnUpdate(void) {
    if (g_invinv_timer > 0) {
        g_invinv_timer--;
        if (g_invinv_timer == 0) {
            g_invinv_flicker = 0;
        }
    }
}

void Damage_InvulnEnd(void) {
    g_invinv_timer = 0;
    g_invinv_flicker = 0;
}

s32 Damage_IsInvuln(void) {
    return g_invinv_timer > 0;
}
