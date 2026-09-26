/* Gameplay level fade to black functions. */

#include "nds_types.h"

void FadeToBlack_Init(void) {
    g_fadetoblack_state = 0;
    g_fade_state = 0;
    g_fade_timer = 0;
}

void FadeToBlack_Start(void) {
    g_fadetoblack_state = 1;
    g_fade_state = 1;
    g_fade_timer = 0;
}

s32 FadeToBlack_Update(void) {
    if (g_fadetoblack_state == 0) return 0;
    g_fade_timer++;
    if (g_fade_timer >= 30) {
        g_fadetoblack_state = 0;
        g_fade_state = 0;
        return 1;
    }
    return 0;
}

s32 FadeToBlack_IsActive(void) {
    return g_fadetoblack_state;
}

void FadeToBlack_Reset(void) {
    g_fadetoblack_state = 0;
    g_fade_state = 0;
    g_fade_timer = 0;
}
