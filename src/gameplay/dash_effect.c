/* Gameplay level dash effect functions. */
#include "nds_types.h"

void DashEffect_Init(void) {
    g_dasheffect_active = 0;
    g_dasheffect_dir = 0;
    g_dasheffect_timer = 0;
}

void DashEffect_Start(void) {
    g_dasheffect_active = 1;
    g_dasheffect_timer = 0;
}

void DashEffect_Update(void) {
    if (!g_dasheffect_active) return;
    g_dasheffect_timer++;
    if (g_dasheffect_timer >= 30) {
        DashEffect_Stop();
    }
}

void DashEffect_Draw(void) { }

void DashEffect_Stop(void) {
    g_dasheffect_active = 0;
}

s32 DashEffect_IsActive(u32 index) {
    (void)index;
    return g_dasheffect_active;
}

u32 DashEffect_GetTimer(void) { return g_dasheffect_timer; }

u32 DashEffect_GetDirection(void) { return g_dasheffect_dir; }

void DashEffect_GetPosition(void) { }

void DashEffect_Reset(void) {
    g_dasheffect_active = 0;
    g_dasheffect_dir = 0;
    g_dasheffect_timer = 0;
}
