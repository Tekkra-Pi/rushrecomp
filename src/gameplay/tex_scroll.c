/* Gameplay level texture scroll functions. */
#include "nds_types.h"


void TexScroll_Init(void) { g_texscroll_count = 0; }

s32 TexScroll_Add(u32 bg_id, s32 speed_x, s32 speed_y, u32 mode) {
    if (g_texscroll_count >= 8) return -1;
    u32 i = g_texscroll_count;
    g_texscrolls[i].bg_id = bg_id; g_texscrolls[i].speed_x = speed_x;
    g_texscrolls[i].speed_y = speed_y; g_texscrolls[i].mode = mode;
    g_texscrolls[i].offset_x = 0; g_texscrolls[i].offset_y = 0;
    g_texscrolls[i].active = 1; g_texscroll_count++; return i;
}

void TexScroll_Update(void) {
    u32 i;
    for (i = 0; i < g_texscroll_count; i++) {
        if (g_texscrolls[i].active) {
            g_texscrolls[i].offset_x += g_texscrolls[i].speed_x;
            g_texscrolls[i].offset_y += g_texscrolls[i].speed_y;
            DisplayScroll_SetBgOffset(g_texscrolls[i].bg_id,
                g_texscrolls[i].offset_x >> 8, g_texscrolls[i].offset_y >> 8);
        }
    }
}

void TexScroll_Remove(u32 i) { if (i < g_texscroll_count) g_texscrolls[i].active = 0; }
u32 TexScroll_GetCount(void) { return g_texscroll_count; }
s32 TexScroll_IsActive(u32 i) { return (i < g_texscroll_count) ? g_texscrolls[i].active : 0; }
void TexScroll_Clear(void) { g_texscroll_count = 0; }
void TexScroll_Reset(void) { g_texscroll_count = 0; }
