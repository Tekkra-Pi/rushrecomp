#include "nds_types.h"
static u32 s_count;
void Hazard_Init(void) { s_count = 0; }
s32 Hazard_Add(s32 x, s32 y, s32 w, s32 h, u32 type) {
    if (s_count >= 16) return -1;
    g_hazards[s_count].x = x; g_hazards[s_count].y = y; g_hazards[s_count].w = w; g_hazards[s_count].h = h;
    g_hazards[s_count].type = type; g_hazards[s_count].active = 1;
    s_count++; return s_count - 1;
}
void Hazard_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_hazards[i].active) continue;
        s32 px, py; Player_GetPosition(&px, &py);
        s32 dx = px - g_hazards[i].x, dy = py - g_hazards[i].y;
        if (dx >= 0 && dx <= g_hazards[i].w && dy >= 0 && dy <= g_hazards[i].h)
            if (!Player_IsInvincible()) Player_Hurt();
    }
}
void Hazard_Remove(u32 index) { if (index < s_count) g_hazards[index].active = 0; }
u32 Hazard_GetCount(void) { return s_count; }
void Hazard_Clear(void) { s_count = 0; }
void Hazard_Reset(void) { s_count = 0; }
