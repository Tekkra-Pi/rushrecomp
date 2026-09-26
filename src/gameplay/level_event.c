#include "nds_types.h"
static u32 s_count;
void LevelEvent_Init(void) { s_count = 0; }
s32 LevelEvent_Add(u32 type, u32 param) {
    if (s_count >= 32) return -1;
    g_stage_events[s_count].id = s_count; g_stage_events[s_count].type = type;
    g_stage_events[s_count].param = param; g_stage_events[s_count].active = 1;
    s_count++; return s_count - 1;
}
void LevelEvent_Update(void) { }
void LevelEvent_Remove(u32 index) { if (index < s_count) g_stage_events[index].active = 0; }
u32 LevelEvent_GetCount(void) { return s_count; }
void LevelEvent_Clear(void) { s_count = 0; }
void LevelEvent_Reset(void) { s_count = 0; }
