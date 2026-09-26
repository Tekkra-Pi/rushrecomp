/* Title screen functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_intro_step;
extern u16 g_key_current, g_key_previous;
extern u32 g_fade_state;
extern u32 g_fade_timer;

void Title_Init(void) {
    g_intro_step = 0;
    g_fade_state = 0;
    g_fade_timer = 0;
    REG_DISPCNT = 0x10100;
    REG_MASTER_BRIGHT = 0;
}

s32 Title_Update(void) {
    u16 pressed = g_key_current & ~g_key_previous;
    switch (g_intro_step) {
        case 0:
            g_fade_state = 1;
            g_fade_timer = 16;
            g_intro_step = 1;
            break;
        case 1:
            if (g_fade_timer == 0) {
                g_intro_step = 2;
            }
            break;
        case 2:
            if (pressed & (KEY_A | KEY_START)) {
                g_intro_step = 3;
                g_fade_state = 2;
                g_fade_timer = 16;
            }
            break;
        case 3:
            if (g_fade_timer == 0) {
                return 1;
            }
            break;
    }
    return 0;
}

void Title_Draw(void) {
    extern void Tilemap_DrawAll(void);
    Tilemap_DrawAll();
}

void Title_LoadGraphics(void) {
    extern void Palette_LoadMain(void);
    Palette_LoadMain();
}

void Title_InitAnimation(void) {
    g_intro_step = 0;
}

void Title_UpdateAnimation(void) {
    /* Update title screen animation frames */
    extern void Tilemap_Update(void);
    Tilemap_Update();
}
