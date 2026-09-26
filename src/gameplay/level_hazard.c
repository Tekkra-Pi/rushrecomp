#include "nds_types.h"
static u32 s_count;
void LevelHazard_Init(void) { s_count = 0; }
s32 LevelHazard_Add(s32 x, s32 y, s32 w, s32 h, u32 type) {
    if (s_count >= 16) return -1;
    g_hazards[s_count].x = x; g_hazards[s_count].y = y; g_hazards[s_count].w = w; g_hazards[s_count].h = h;
    g_hazards[s_count].type = type; g_hazards[s_count].active = 1;
    s_count++; return s_count - 1;
}
void LevelHazard_Update(void) { }
void LevelHazard_Remove(u32 index) { if (index < s_count) g_hazards[index].active = 0; }
u32 LevelHazard_GetCount(void) { return s_count; }
void LevelHazard_Clear(void) { s_count = 0; }
void LevelHazard_Reset(void) { s_count = 0; }
