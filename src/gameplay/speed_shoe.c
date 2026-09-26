#include "nds_types.h"
static u32 s_count;
void SpeedShoe_Init(void) { s_count = 0; g_speedshoe_count = 0; }
s32 SpeedShoe_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y; (void)param;
    if (s_count >= 4) return -1;
    s_count++; g_speedshoe_count = s_count; return s_count - 1;
}
void SpeedShoe_Update(void) { }
void SpeedShoe_Remove(u32 index) { (void)index; }
u32 SpeedShoe_GetCount(void) { return s_count; }
void SpeedShoe_Clear(void) { s_count = 0; g_speedshoe_count = 0; }
void SpeedShoe_Reset(void) { SpeedShoe_Clear(); }
