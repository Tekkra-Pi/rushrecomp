#include "nds_types.h"
static u32 s_count;
void StageEvent_Init(void) { s_count = 0; }
s32 StageEvent_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_count >= 32) return -1;
    s_count++; return s_count - 1;
}
void StageEvent_Update(void) { }
void StageEvent_Remove(u32 index) { (void)index; }
u32 StageEvent_GetCount(void) { return s_count; }
void StageEvent_Clear(void) { s_count = 0; }
void StageEvent_Reset(void) { s_count = 0; }
