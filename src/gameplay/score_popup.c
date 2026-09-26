#include "nds_types.h"
static u32 s_count;
void ScorePopup_Init(void) { s_count = 0; }
s32 ScorePopup_Spawn(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_count >= 16) return -1;
    s_count++; return s_count - 1;
}
void ScorePopup_Update(void) { }
void ScorePopup_Draw(void) { }
u32 ScorePopup_GetCount(void) { return s_count; }
void ScorePopup_Clear(void) { s_count = 0; }
void ScorePopup_Reset(void) { s_count = 0; }
