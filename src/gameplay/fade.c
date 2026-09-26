/* Gameplay level fade functions. */
#include "nds_types.h"


void Fade_Init(void) { g_fade_state = 0; g_fade_timer = 0; g_fade_alpha = 0; }

void Fade_StartOut(void) { g_fade_state = 1; g_fade_timer = 0; }
void Fade_StartIn(void) { g_fade_state = 3; g_fade_timer = 0; }

s32 Fade_Update(void) {
    switch (g_fade_state) {
        case 0: return 1;
        case 1: /* fading out */
            g_fade_timer++;
            g_fade_alpha = g_fade_timer * 16;
            if (g_fade_alpha >= 256) { g_fade_alpha = 256; g_fade_state = 2; }
            Display_SetMasterBright(g_fade_alpha > 16 ? 16 : g_fade_alpha);
            return 0;
        case 2: /* fully out */
            return 1;
        case 3: /* fading in */
            g_fade_timer++;
            g_fade_alpha = 256 - g_fade_timer * 16;
            if (g_fade_alpha <= 0) { g_fade_alpha = 0; g_fade_state = 0; }
            Display_SetMasterBright(g_fade_alpha > 16 ? 16 : g_fade_alpha);
            return 0;
    }
    return 1;
}

u32 Fade_GetState(void) { return g_fade_state; }
s32 Fade_GetAlpha(void) { return g_fade_alpha; }
void Fade_Reset(void) { g_fade_state = 0; g_fade_timer = 0; g_fade_alpha = 0; }
