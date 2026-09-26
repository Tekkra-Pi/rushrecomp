/* Gameplay level combo counter functions. */
#include "nds_types.h"

void ComboCounter_Init(void) { g_combo_counter = 0; g_combo_timer = 0; g_combo_max = 0; g_combo_active = 0; }

void ComboCounter_Add(u32 count) {
    g_combo_counter += count;
    g_combo_timer = 120;
    g_combo_active = 1;
    if (g_combo_counter > g_combo_max) g_combo_max = g_combo_counter;
}

void ComboCounter_Update(void) {
    if (!g_combo_active) return;
    if (g_combo_timer > 0) {
        g_combo_timer--;
    } else {
        g_combo_counter = 0;
        g_combo_active = 0;
    }
}

u32 ComboCounter_Get(void) { return g_combo_counter; }
u32 ComboCounter_GetMax(void) { return g_combo_max; }
s32 ComboCounter_IsActive(void) { return g_combo_active; }
void ComboCounter_Reset(void) { g_combo_counter = 0; g_combo_timer = 0; g_combo_max = 0; g_combo_active = 0; }
