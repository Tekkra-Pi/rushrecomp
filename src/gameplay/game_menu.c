/* Gameplay menu functions. */
#include "nds_types.h"

static u32 s_active;

void Menu_Init(void) { s_active = 0; g_menu_index = 0; }
void Menu_RemoveItem(void) { if (g_menu_index > 0) g_menu_index--; }
void Menu_Update(void) {
    if (!s_active) return;
    if (g_key_pressed & (1 << 5)) { if (g_menu_index > 0) g_menu_index--; }
    if (g_key_pressed & (1 << 4)) { g_menu_index++; if (g_menu_index >= 4) g_menu_index = 3; }
}
void Menu_Select(void) {
    if (!s_active) return;
    if (g_menu_index < 4 && g_menu_items[g_menu_index].callback)
        g_menu_items[g_menu_index].callback();
}
void Menu_Draw(void) {
    if (!s_active) return;
    u32 i;
    for (i = 0; i < 4; i++)
        if (i == g_menu_index) Display_PrintFixed(80, 80 + i * 20, ">");
}
void Menu_Open(void) { s_active = 1; g_menu_index = 0; }
void Menu_Close(void) { s_active = 0; }
s32 Menu_IsActive(u32 index) { (void)index; return s_active; }
u32 Menu_GetIndex(void) { return g_menu_index; }
u32 Menu_GetCount(void) { return 4; }
void Menu_Clear(void) { s_active = 0; g_menu_index = 0; }
void Menu_Reset(void) { Menu_Clear(); }
