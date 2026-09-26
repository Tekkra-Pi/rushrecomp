#include "nds_types.h"
static u32 s_count;
void MenuText_Init(void) { s_count = 0; }
s32 MenuText_Add(s32 x, s32 y, u32 str_id) {
    if (s_count >= 16) return -1;
    g_menutexts[s_count].x = x; g_menutexts[s_count].y = y;
    g_menutexts[s_count].str_id = str_id; g_menutexts[s_count].active = 1;
    s_count++; return s_count - 1;
}
void MenuText_Update(void) { }
void MenuText_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_menutexts[i].active) Display_PrintFixed(g_menutexts[i].x, g_menutexts[i].y, "TEXT");
}
void MenuText_Remove(u32 index) { if (index < s_count) g_menutexts[index].active = 0; }
u32 MenuText_GetCount(void) { return s_count; }
void MenuText_Clear(void) { s_count = 0; }
void MenuText_Reset(void) { s_count = 0; }
