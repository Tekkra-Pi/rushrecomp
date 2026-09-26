/* Display effect functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_fadetoblack_state;
extern u32 g_fade_state;
extern u32 g_fade_timer;
extern u32 g_fade_alpha;

void DisplayEffect_Init(void) {
    g_fadetoblack_state = 0;
    g_fade_state = 0;
    g_fade_timer = 0;
    g_fade_alpha = 0;
    g_dispeffect_count = 0;
}

void DisplayEffect_Play(u32 index) {
    (void)index;
    g_fade_state = 1;
    g_fade_timer = 16;
}

void DisplayEffect_Update(void) {
    if (g_fade_timer > 0) {
        g_fade_timer--;
    }
}

void DisplayEffect_FadeToBlack(void) {
    g_fadetoblack_state = 1;
    g_fade_state = 1;
    g_fade_timer = 16;
}

void DisplayEffect_FadeFromBlack(void) {
    g_fadetoblack_state = 2;
    g_fade_state = 2;
    g_fade_timer = 16;
}

void DisplayEffect_Flash(void) {
    REG_MASTER_BRIGHT = 0x8010;
    g_fade_timer = 4;
}

void DisplayEffect_Wipe(void) {
    g_fade_state = 3;
    g_fade_timer = 32;
}
