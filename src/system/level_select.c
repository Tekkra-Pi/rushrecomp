/* Level select functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_menu_index;
extern u16 g_key_current, g_key_previous;

static const u32 level_zones[] = {0, 1, 2, 3, 4, 5, 6, 7};
static const u32 level_acts[]  = {0, 0, 0, 0, 0, 0, 0, 0};
#define LEVEL_COUNT 8

void LevelSelect_Init(void) {
    g_menu_index = 0;
}

s32 LevelSelect_Update(void) {
    u16 pressed = g_key_current & ~g_key_previous;
    if (pressed & KEY_UP) {
        if (g_menu_index > 0) g_menu_index--;
    }
    if (pressed & KEY_DOWN) {
        if (g_menu_index < LEVEL_COUNT - 1) g_menu_index++;
    }
    if (pressed & (KEY_A | KEY_START)) {
        extern void ZoneMgr_LoadZone(u32 zone, u32 act);
        ZoneMgr_LoadZone(level_zones[g_menu_index], level_acts[g_menu_index]);
        return 1;
    }
    return 0;
}

void LevelSelect_Draw(void) {
    /* HUD draws level names via OAM text sprites */
}

void LevelSelect_LoadGraphics(void) {
    extern void Palette_LoadMain(void);
    Palette_LoadMain();
}

void LevelSelect_InitList(void) {
    g_menu_index = 0;
}
