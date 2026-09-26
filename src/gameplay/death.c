/* Death and respawn functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern void Player_SetPosition(s32 x, s32 y);
extern void Player_SetState(u32 state);
extern void Display_FadeToBlack(void);
extern void Display_FadeFromBlack(void);

static void Death_RespawnInternal(void);

void Death_Init(void) {
    g_deathseq_timer = 0;
}

void Death_Update(void) {
    if (g_player_state == 0) return;
    g_deathseq_timer++;
    if (g_deathseq_timer == 1) {
        Display_FadeToBlack();
    }
    if (g_deathseq_timer >= 60) {
        Death_RespawnInternal();
    }
}

static void Death_RespawnInternal(void) {
    g_lives--;
    g_deathseq_timer = 0;
    g_player_state = 0;
    Player_SetPosition(g_respawn_x, g_respawn_y);
    Player_SetState(0);
    Display_FadeFromBlack();
    g_game_rings = g_levelsave_rings;
    g_game_score = g_levelsave_score;
}

void Death_Respawn(void) {
    Death_RespawnInternal();
}

s32 Death_IsDead(void) {
    return g_player_state != 0;
}

void Death_GameOver(void) {
    g_gamestate_state = 5;
}
