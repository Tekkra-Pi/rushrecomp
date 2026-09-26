#include "nds_types.h"
static u32 s_count;
void RingLayout_Init(void) { s_count = 0; }
s32 RingLayout_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_count >= 32) return -1;
    s_count++; return s_count - 1;
}
void RingLayout_Remove(u32 index) { (void)index; }
s32 RingLayout_Check(void) { return 0; }
u32 RingLayout_GetPattern(void) { return 0; }
u32 RingLayout_GetParam(void) { return 0; }
s32 RingLayout_IsActive(u32 index) { (void)index; return s_count > 0 ? 1 : 0; }
u32 RingLayout_GetCount(void) { return s_count; }
void RingLayout_Clear(void) { s_count = 0; }
void RingLayout_Reset(void) { s_count = 0; }
