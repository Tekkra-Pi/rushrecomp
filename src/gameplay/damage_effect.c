/* Gameplay level damage effect functions. */
#include "nds_types.h"

void DamageEffect_Init(void) {
    g_dmgfx_dir_x = 0;
    g_dmgfx_dir_y = 0;
    g_dmgfx_timer = 0;
}

void DamageEffect_Start(void) {
    g_dmgfx_timer = 20;
}

void DamageEffect_Update(void) {
    if (g_dmgfx_timer > 0) g_dmgfx_timer--;
}

void DamageEffect_Stop(void) {
    g_dmgfx_timer = 0;
}

s32 DamageEffect_IsActive(u32 index) {
    (void)index;
    return g_dmgfx_timer > 0;
}

u32 DamageEffect_GetTimer(void) { return g_dmgfx_timer; }

void DamageEffect_GetDirection(void) { }

s32 DamageEffect_ShouldFlicker(void) {
    return (g_dmgfx_timer & 2) != 0;
}

u32 DamageEffect_GetProgress(void) { return g_dmgfx_timer; }

void DamageEffect_Reset(void) {
    g_dmgfx_dir_x = 0;
    g_dmgfx_dir_y = 0;
    g_dmgfx_timer = 0;
}
