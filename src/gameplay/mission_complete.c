#include "nds_types.h"
static u32 s_state, s_timer;
void MissionComplete_Init(void) { s_state = 0; s_timer = 0; }
void MissionComplete_Start(void) { s_state = 1; s_timer = 0; }
s32 MissionComplete_Update(void) {
    if (s_state == 0) return 0;
    s_timer++; if (s_timer >= 120) { s_state = 0; return 1; } return 0;
}
void MissionComplete_Draw(void) { if (s_state) Display_PrintFixed(80, 80, "MISSION COMPLETE"); }
u32 MissionComplete_GetState(void) { return s_state; }
void MissionComplete_Reset(void) { s_state = 0; s_timer = 0; }
