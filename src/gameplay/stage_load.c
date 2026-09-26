#include "nds_types.h"
static u32 s_state;
void StageLoad_Init(void) { s_state = 0; g_stageload_state = 0; }
void StageLoad_Start(void) { s_state = 1; g_stageload_state = 1; g_stage_load_progress = 0; }
s32 StageLoad_Update(void) {
    if (s_state == 0) return 0;
    g_stage_load_progress++;
    if (g_stage_load_progress >= 60) { s_state = 0; g_stageload_state = 0; return 1; }
    return 0;
}
u32 StageLoad_GetState(void) { return s_state; }
void StageLoad_Reset(void) { s_state = 0; g_stageload_state = 0; }
