#include "nds_types.h"
void MissionEval_Init(void) { g_missioneval_count = 0; }
s32 MissionEval_Add(u32 type, u32 param) {
    (void)type; (void)param;
    if (g_missioneval_count >= 8) return -1;
    g_missioneval_count++; return g_missioneval_count - 1;
}
void MissionEval_Update(void) { }
void MissionEval_Remove(u32 index) { (void)index; }
u32 MissionEval_GetCount(void) { return g_missioneval_count; }
void MissionEval_Clear(void) { g_missioneval_count = 0; }
void MissionEval_Reset(void) { g_missioneval_count = 0; }
