/* Gameplay level menu cursor functions. */
#include "nds_types.h"


void MenuCursor_Init(void) { g_menucursor_count = 0; }

s32 MenuCursor_Add(s32 x, s32 y, u32 menu_id) {
    if (g_menucursor_count >= 4) return -1;
    u32 i = g_menucursor_count;
    g_menucursors[i].x = x; g_menucursors[i].y = y;
    g_menucursors[i].menu_id = menu_id; g_menucursors[i].selected = 0;
    g_menucursors[i].active = 1;
    g_menucursor_count++; return i;
}

void MenuCursor_Update(void) {
    u32 i;
    for (i = 0; i < g_menucursor_count; i++) {
        if (!g_menucursors[i].active) continue;
        u16 keys = Input_GetPressed();
        if (keys & KEY_UP) {
            g_menucursors[i].selected = (g_menucursors[i].selected > 0) ?
                g_menucursors[i].selected - 1 : g_menucursors[i].max_items;
        }
        if (keys & KEY_DOWN) {
            g_menucursors[i].selected = (g_menucursors[i].selected < g_menucursors[i].max_items) ?
                g_menucursors[i].selected + 1 : 0;
        }
    }
}

void MenuCursor_Draw(void) {
    u32 i;
    for (i = 0; i < g_menucursor_count; i++) {
        if (g_menucursors[i].active) {
            s32 draw_y = g_menucursors[i].y + g_menucursors[i].selected * 24;
            OAM_AddSprite(g_menucursors[i].x, draw_y, 0x3a4, 0x1000);
        }
    }
}

void MenuCursor_Remove(u32 i) { if (i < g_menucursor_count) g_menucursor_count--; }
u32 MenuCursor_GetCount(void) { return g_menucursor_count; }
void MenuCursor_Clear(void) { g_menucursor_count = 0; }
void MenuCursor_Reset(void) { g_menucursor_count = 0; }
