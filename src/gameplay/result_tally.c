#include "nds_types.h"
void ResultTally_Init(void) { g_resulttally_state = 0; g_resulttally_timer = 0; }
void ResultTally_Start(void) { g_resulttally_state = 1; g_resulttally_timer = 0; }
s32 ResultTally_Update(void) {
    if (g_resulttally_state == 0) return 0;
    g_resulttally_timer++;
    if (g_resulttally_timer >= 300) { g_resulttally_state = 0; return 1; }
    return 0;
}
void ResultTally_Draw(void) { if (g_resulttally_state) Display_PrintFixed(80, 40, "RESULT TALLY"); }
void ResultTally_Skip(void) { g_resulttally_state = 0; }
u32 ResultTally_GetState(void) { return g_resulttally_state; }
u32 ResultTally_GetTimer(void) { return g_resulttally_timer; }
void ResultTally_Reset(void) { g_resulttally_state = 0; g_resulttally_timer = 0; }
