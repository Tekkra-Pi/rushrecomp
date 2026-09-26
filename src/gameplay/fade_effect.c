/* Gameplay level fade effect functions. */
#include "nds_types.h"

static u32 s_fadeeffect_active;
static u32 s_fadeeffect_timer;
static u32 s_fadeeffect_type;

void FadeEffect_Init(void) {
    s_fadeeffect_active = 0;
    s_fadeeffect_timer = 0;
    s_fadeeffect_type = 0;
    g_palfade_active = 0;
    g_palfade_timer = 0;
}

void FadeEffect_Start(void) {
    s_fadeeffect_active = 1;
    s_fadeeffect_timer = 0;
    g_palfade_active = 1;
    g_palfade_timer = 0;
}

void FadeEffect_Update(void) {
    if (!s_fadeeffect_active) return;
    s_fadeeffect_timer++;
    g_palfade_timer = s_fadeeffect_timer;
    if (s_fadeeffect_timer >= 30) {
        s_fadeeffect_active = 0;
        g_palfade_active = 0;
    }
}

void FadeEffect_Stop(void) {
    s_fadeeffect_active = 0;
    g_palfade_active = 0;
}

s32 FadeEffect_IsActive(u32 index) {
    (void)index;
    return s_fadeeffect_active;
}

u32 FadeEffect_GetTimer(void) { return s_fadeeffect_timer; }

void FadeEffect_GetProgress(void) { }

void FadeEffect_Draw(void) { }

void FadeEffect_SetType(u32 index, u32 value) {
    (void)index;
    s_fadeeffect_type = value;
}

void FadeEffect_Reset(void) {
    s_fadeeffect_active = 0;
    s_fadeeffect_timer = 0;
    s_fadeeffect_type = 0;
    g_palfade_active = 0;
    g_palfade_timer = 0;
}
