#include "nds_types.h"
static u32 s_count;
void RingAnim_Init(void) { s_count = 0; }
s32 RingAnim_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_count >= 32) return -1;
    s_count++; return s_count - 1;
}
void RingAnim_Update(void) { }
void RingAnim_Draw(void) { }
void RingAnim_Remove(u32 index) { (void)index; }
u32 RingAnim_GetCount(void) { return s_count; }
s32 RingAnim_IsActive(u32 index) { (void)index; return s_count > 0 ? 1 : 0; }
void RingAnim_Clear(void) { s_count = 0; }
