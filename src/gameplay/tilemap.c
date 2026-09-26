/* Gameplay level tilemap draw functions. */
#include "nds_types.h"


void Tilemap_DrawAll(void) {
    s32 cx, cy;
    DisplayScroll_GetPosition(&cx, &cy);
    u32 bg;
    for (bg = 0; bg < 4; bg++) {
        u32 scroll_x = (cx * g_tilemap_parallax[bg]) / 256;
        u32 scroll_y = (cy * g_tilemap_parallax_y[bg]) / 256;
        DisplayScroll_SetBgOffset(bg, scroll_x, scroll_y);
    }
}

void Tilemap_SetParallax(u32 bg, s32 factor_x, s32 factor_y) {
    if (bg < 4) {
        g_tilemap_parallax[bg] = factor_x;
        g_tilemap_parallax_y[bg] = factor_y;
    }
}

void Tilemap_Init(void) {
    u32 i;
    for (i = 0; i < 4; i++) {
        g_tilemap_parallax[i] = 256;
        g_tilemap_parallax_y[i] = 256;
    }
}

void Tilemap_Reset(void) { Tilemap_Init(); }
