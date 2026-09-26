/* Gameplay score popup functions. */
#include "nds_types.h"

static u32 s_count;

void Popup_Init(void) { s_count = 0; }
s32 Popup_Create(void) {
    if (s_count >= 16) return -1;
    g_popups[s_count].state = 1; g_popups[s_count].str_id = 0;
    g_popups[s_count].timer = 0; g_popups[s_count].active = 1;
    s_count++; return s_count - 1;
}
void Popup_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_popups[i].active) continue;
        g_popups[i].timer++;
        if (g_popups[i].timer >= 60) g_popups[i].active = 0;
    }
}
void Popup_Draw(void) { }
void Popup_Remove(u32 index) { if (index < s_count) g_popups[index].active = 0; }
u32 Popup_GetCount(void) { return s_count; }
s32 Popup_IsActive(u32 index) { return index < s_count ? g_popups[index].active : 0; }
u32 Popup_GetValue(void) { return s_count > 0 ? g_popups[0].str_id : 0; }
void Popup_Clear(void) { s_count = 0; }
