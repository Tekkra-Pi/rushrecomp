#include "nds_types.h"
void MissionFail_Init(void) { g_missionfail_timer = 0; g_missionfail_state = 0; }
void MissionFail_Start(void) { g_missionfail_state = 1; g_missionfail_timer = 0; }
s32 MissionFail_Update(void) {
    if (g_missionfail_state == 0) return 0;
    g_missionfail_timer++;
    if (g_missionfail_timer >= 120) { g_missionfail_state = 0; return 1; }
    return 0;
}
void MissionFail_Draw(void) { if (g_missionfail_state) Display_PrintFixed(80, 80, "MISSION FAILED"); }
u32 MissionFail_GetState(void) { return g_missionfail_state; }
void MissionFail_Reset(void) { g_missionfail_timer = 0; g_missionfail_state = 0; }
