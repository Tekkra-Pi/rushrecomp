#include "nds_types.h"
static u32 s_count;
void ItemMonitor_Init(void) { s_count = 0; }
s32 ItemMonitor_Add(s32 x, s32 y, u32 type) {
    if (s_count >= 32) return -1;
    g_itemmonitors[s_count].x = x; g_itemmonitors[s_count].y = y;
    g_itemmonitors[s_count].type = type; g_itemmonitors[s_count].active = 1;
    s_count++; return s_count - 1;
}
void ItemMonitor_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_itemmonitors[i].active) continue;
        s32 px, py; Player_GetPosition(&px, &py);
        s32 dx = px - g_itemmonitors[i].x, dy = py - g_itemmonitors[i].y;
        if (dx >= -0x400 && dx <= 0x400 && dy >= -0x400 && dy <= 0x400) {
            PowerUp_Spawn(g_itemmonitors[i].x, g_itemmonitors[i].y - 0x200, g_itemmonitors[i].type);
            g_itemmonitors[i].active = 0; SoundEffect_Play(0xd1, 0x40, 0x100);
        }
    }
}
void ItemMonitor_Remove(u32 index) { if (index < s_count) g_itemmonitors[index].active = 0; }
u32 ItemMonitor_GetCount(void) { return s_count; }
void ItemMonitor_Clear(void) { s_count = 0; }
void ItemMonitor_Reset(void) { s_count = 0; }
