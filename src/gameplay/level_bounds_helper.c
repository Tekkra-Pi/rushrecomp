/* Gameplay level level bounds helper functions. */
#include "nds_types.h"


void LevelBoundsHelper_Init(void) {
    g_levelbounds_left = 0;
    g_levelbounds_right = 0x80000;
    g_levelbounds_top = 0;
    g_levelbounds_bottom = 0x10000;
}

void LevelBoundsHelper_Set(s32 left, s32 right, s32 top, s32 bottom) {
    g_levelbounds_left = left;
    g_levelbounds_right = right;
    g_levelbounds_top = top;
    g_levelbounds_bottom = bottom;
}

void LevelBoundsHelper_Update(void) {
    s32 px, py;
    Player_GetPosition(&px, &py);
    if (px < g_levelbounds_left) Player_SetPosition(g_levelbounds_left, py);
    if (px > g_levelbounds_right) Player_SetPosition(g_levelbounds_right, py);
    if (py < g_levelbounds_top) Player_SetPosition(px, g_levelbounds_top);
    if (py > g_levelbounds_bottom) Player_Kill();
}

s32 LevelBoundsHelper_GetLeft(void) { return g_levelbounds_left; }
s32 LevelBoundsHelper_GetRight(void) { return g_levelbounds_right; }
s32 LevelBoundsHelper_GetTop(void) { return g_levelbounds_top; }
s32 LevelBoundsHelper_GetBottom(void) { return g_levelbounds_bottom; }
void LevelBoundsHelper_Reset(void) { LevelBoundsHelper_Init(); }
