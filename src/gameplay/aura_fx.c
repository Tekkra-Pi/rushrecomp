/* Gameplay level aura FX functions. */

#include "nds_types.h"

static u32 s_aurafx_active;
static u32 s_aurafx_timer;

void AuraFX_Init(void) {
    s_aurafx_active = 0;
    s_aurafx_timer = 0;
}

void AuraFX_Start(void) {
    s_aurafx_active = 1;
    s_aurafx_timer = 0;
}

void AuraFX_Stop(void) {
    s_aurafx_active = 0;
}

void AuraFX_Update(void) {
    if (!s_aurafx_active) return;
    s_aurafx_timer++;
}

void AuraFX_Draw(void) {
}

s32 AuraFX_IsActive(u32 index) {
    (void)index;
    return s_aurafx_active;
}

void AuraFX_Reset(void) {
    s_aurafx_active = 0;
    s_aurafx_timer = 0;
}
