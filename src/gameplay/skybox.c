/* Gameplay level skybox functions. */
#include "nds_types.h"


void Skybox_Init(void) { g_skybox_tile = 0; g_skybox_scroll_x = 0; g_skybox_scroll_y = 0; }

void Skybox_SetTile(u32 tile) { g_skybox_tile = tile; }

void Skybox_SetScroll(s32 x, s32 y) { g_skybox_scroll_x = x; g_skybox_scroll_y = y; }

void Skybox_Update(void) {
    s32 cx, cy;
    DisplayScroll_GetPosition(&cx, &cy);
    u32 sx = (cx / 4 + g_skybox_scroll_x) & 0xff;
    u32 sy = (cy / 4 + g_skybox_scroll_y) & 0xff;
    BG_SetScroll(0, sx, sy);
}

void Skybox_Draw(void) {
    Display_FillRect(0, 0, 256, 192, g_skybox_tile);
}

void Skybox_Reset(void) { g_skybox_tile = 0; g_skybox_scroll_x = 0; g_skybox_scroll_y = 0; }
