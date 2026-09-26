/* Gameplay combo functions. */
#include "nds_types.h"

void GameCombo_Init(void) { g_combo_counter = 0; g_combo_timer = 0; g_combo_active = 0; }
void GameCombo_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y;
    g_combo_counter += param; g_combo_timer = 120; g_combo_active = 1;
}
void GameCombo_Update(void) {
    if (!g_combo_active) return;
    if (g_combo_timer > 0) g_combo_timer--;
    else { g_combo_counter = 0; g_combo_active = 0; }
}
void GameCombo_Reset(void) { g_combo_counter = 0; g_combo_timer = 0; g_combo_active = 0; }
void GameCombo_UpdateMultiplier(void) {
    if (g_combo_counter > 0) {
        g_score_multiplier = 1 + g_combo_counter / 5;
        if (g_score_multiplier > 8) g_score_multiplier = 8;
    }
}
u32 GameCombo_GetCount(void) { return g_combo_counter; }
u32 GameCombo_GetMultiplier(void) { return g_score_multiplier; }
u32 GameCombo_GetMax(void) { return g_combo_max; }
u32 GameCombo_GetTimer(void) { return g_combo_timer; }
s32 GameCombo_IsActive(u32 index) { (void)index; return g_combo_active; }
