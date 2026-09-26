#include "nds_types.h"
void SpecialStage_Init(void) { g_special_stage_state = 0; }
s32 SpecialStage_Update(void) {
    if (g_special_stage_state == 0) return 0;
    return 0;
}
void SpecialStage_Draw(void) { }
void SpecialStage_LoadData(void) { g_special_stage_state = 1; }
