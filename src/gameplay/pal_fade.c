#include "nds_types.h"
void PalFade_Init(void) { g_palfade_active = 0; g_palfade_timer = 0; }
void PalFade_Start(void) { g_palfade_active = 1; g_palfade_timer = 0; }
void PalFade_Stop(void) { g_palfade_active = 0; }
void PalFade_Update(void) {
    if (!g_palfade_active) return;
    g_palfade_timer++;
    if (g_palfade_timer >= g_palfade_duration) g_palfade_active = 0;
}
s32 PalFade_IsActive(u32 index) { (void)index; return g_palfade_active; }
void PalFade_Reset(void) { g_palfade_active = 0; g_palfade_timer = 0; }
